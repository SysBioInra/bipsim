
from __future__ import absolute_import, division, print_function
from lxml import etree

from src.copasi import CopasiTask, CopasiReport


EXPECTED_COPASI_TASK = """<Task key="Task_benchmark" name="Time-Course" type="timeCourse" scheduled="true" updateModel="false">
  <Report reference="Report_benchmark" target="chemicals.out" append="0" confirmOverwrite="0"/>
  <Problem>
    <Parameter name="AutomaticStepSize" type="bool" value="0"/>
    <Parameter name="StepNumber" type="unsignedInteger" value="100"/>
    <Parameter name="StepSize" type="float" value="0.1"/>
    <Parameter name="Duration" type="float" value="10"/>
    <Parameter name="TimeSeriesRequested" type="bool" value="1"/>
    <Parameter name="OutputStartTime" type="float" value="0"/>
    <Parameter name="Output Event" type="bool" value="0"/>
    <Parameter name="Start in Steady State" type="bool" value="0"/>
  </Problem>
  <Method name="Stochastic (Gibson + Bruck)" type="DirectMethod">
    <Parameter name="Max Internal Steps" type="integer" value="1000000"/>
    <Parameter name="Subtype" type="unsignedInteger" value="2"/>
    <Parameter name="Use Random Seed" type="bool" value="1"/>
    <Parameter name="Random Seed" type="unsignedInteger" value="1"/>
  </Method>
</Task>
"""

EXPECTED_COPASI_REPORT = """<Report key="Report_benchmark" name="Time, Particle Numbers, Volumes, and Global Quantity Values" taskType="timeCourse" separator="&#9;" precision="6">
  <Comment>A table of time, variable species particle numbers, variable compartment volumes, and variable global quantity values.</Comment>
  <Table printTitle="1">
    <Object cn="CN=Root,Model=NoName,Reference=Time"/>
    <Object cn="CN=Root,Model=NoName,Vector=Compartments[cytosol],Vector=Metabolites[A_0],Reference=ParticleNumber"/>
    <Object cn="CN=Root,Model=NoName,Vector=Compartments[cytosol],Vector=Metabolites[A_9],Reference=ParticleNumber"/>
  </Table>
</Report>
"""


def test_copasi_task_creation_10_cascade():
    task = CopasiTask(10)
    assert(etree.tostring(task._xml, pretty_print=True).decode()
           == EXPECTED_COPASI_TASK)


def test_copasi_report_creation_10_cascade():
    report = CopasiReport(10)
    assert(etree.tostring(report._xml, pretty_print=True).decode()
           == EXPECTED_COPASI_REPORT)
