$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCBPM]
$ @BUILD
$ cd [---.lib.SLCBPM]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
