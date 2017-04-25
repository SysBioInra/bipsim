
Inefficient transcription switching
------------------------------------
Transcription termination switching is inefficient. We have two ProductLoading
reactions: one for normal RNA polymerases, other for RNA polymerases that
were blocked on a switch site but finally move on (because it was not their
termination site). ProductLoading reactions are expensive to update. 
Duplicating them is not a good idea. It would be better to change the switch
in a way that another less expensive reaction gets duplicated.
