# Add dependencies
for(dep, DEPENDENCY_LIBRARIES) {
    #message($$TARGET depends on $$dep ($${dep}/libasc$${dep}.a))
    LIBS += ../$${dep}/libasc$${dep}.a
    PRE_TARGETDEPS += ../$${dep}/libasc$${dep}.a
}
