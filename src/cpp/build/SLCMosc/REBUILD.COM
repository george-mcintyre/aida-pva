$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCMOSC]
$ @BUILD
$ cd [---.lib.SLCMOSC]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
