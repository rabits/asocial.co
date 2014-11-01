# Add dependencies
for(dep, DEPENDENCY_LIBRARIES) {
    message($$TARGET depends on $$dep ($${dep}/lib$${dep}.so))
    LIBS += ../$${dep}/lib$${dep}.so
    PRE_TARGETDEPS += ../$${dep}/lib$${dep}.so
}
