$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.REFERENCE]
$ @BUILD
$ cd [---.lib.REFERENCE]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
