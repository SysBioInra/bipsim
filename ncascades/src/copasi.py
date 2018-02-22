
from __future__ import absolute_import, division, print_function
import os.path
from lxml import etree


EXE_NAME = '/Applications/COPASI/CopasiSE'


def write_run_script(output_dir, input_file):
    filename = os.path.join(output_dir, 'run.sh')
    with open(filename, 'w') as output:
        output.write(EXE_NAME + ' ' + input_file)


class CopasiModel(object):
    def __init__(self, base_model):
        parser = etree.XMLParser(remove_blank_text=True)
        self._document = etree.parse(base_model, parser)
        self._namespace = list(self._document.getroot().nsmap.values())[0]

    def add_n_cascade(self, size):
        self._remove_time_course()
        self._find('ListOfTasks').extend([CopasiTask(size)._xml])
        self._find('ListOfReports').extend([CopasiReport(size)._xml])

    def _remove_time_course(self):
        for task in self._find('ListOfTasks'):
            if task.get('type') == 'timeCourse':
                task.getparent().remove(task)

    def _find(self, tag):
        return self._document.find('{' + self._namespace + '}' + tag)

    def write(self, output):
        self._document.write(output, pretty_print=True)


class CopasiTask(object):
    def __init__(self, max_time):
        self._max_time = max_time
        self._xml = self._create_task()
        self._xml.extend([
            self._create_report_reference(),
            self._create_problem(),
            self._create_method()
        ])

    def _create_task(self):
        result = etree.Element('Task')
        result.set('key', 'Task_benchmark')
        result.set('name', 'Time-Course')
        result.set('type', 'timeCourse')
        result.set('scheduled', 'true')
        result.set('updateModel', 'false')
        return result

    def _create_report_reference(self):
        result = etree.Element('Report')
        result.set('reference', 'Report_benchmark')
        result.set('target', 'chemicals.out')
        result.set('append', '0')
        result.set('confirmOverwrite', '0')
        return result

    def _create_problem(self):
        result = etree.Element('Problem')
        result.extend([
            self._create_parameter('AutomaticStepSize', 'bool', 0),
            self._create_parameter('StepNumber', 'unsignedInteger', 100),
            self._create_parameter('StepSize', 'float', self._max_time/100),
            self._create_parameter('Duration', 'float', self._max_time),
            self._create_parameter('TimeSeriesRequested', 'bool', 1),
            self._create_parameter('OutputStartTime', 'float', 0),
            self._create_parameter('Output Event', 'bool', 0),
            self._create_parameter('Start in Steady State', 'bool', 0)
        ])
        return result

    def _create_parameter(self, name, type_, value):
        result = etree.Element('Parameter')
        result.set('name', name)
        result.set('type', type_)
        result.set('value', str(value))
        return result

    def _create_method(self):
        result = etree.Element('Method')
        result.set('name', 'Stochastic (Gibson + Bruck)')
        result.set('type', 'DirectMethod')
        result.extend([
            self._create_parameter('Max Internal Steps', 'integer', '1000000'),
            self._create_parameter('Subtype', 'unsignedInteger', '2'),
            self._create_parameter('Use Random Seed', 'bool', '1'),
            self._create_parameter('Random Seed', 'unsignedInteger', '1')
        ])
        return result


class CopasiReport(object):
    def __init__(self, cascade_size):
        self._xml = self._create_report()
        self._xml.extend([
            self._create_comment(),
            self._create_table(cascade_size)
        ])

    def _create_report(self):
        result = etree.Element('Report')
        result.set('key', 'Report_benchmark')
        result.set('name', 'Time, Particle Numbers, Volumes, '
                   'and Global Quantity Values')
        result.set('taskType', 'timeCourse')
        result.set('separator', '\t')
        result.set('precision', '6')
        return result

    def _create_comment(self):
        result = etree.Element('Comment')
        result.text = ('A table of time, variable species particle numbers, '
                       'variable compartment volumes, '
                       'and variable global quantity values.')
        return result

    def _create_table(self, cascade_size):
        result = etree.Element('Table')
        result.set('printTitle', '1')
        result.extend([
            self._create_time_object(),
            self._create_metabolite_object('A_0'),
            self._create_metabolite_object('A_' + str(cascade_size-1))
        ])
        return result

    def _create_time_object(self):
        result = etree.Element('Object')
        result.set('cn', 'CN=Root,Model=NoName,Reference=Time')
        return result

    def _create_metabolite_object(self, metabolite):
        result = etree.Element('Object')
        result.set('cn',
                   'CN=Root,Model=NoName,Vector=Compartments[cytosol],'
                   'Vector=Metabolites[{}],Reference=ParticleNumber'
                   .format(metabolite))
        return result
