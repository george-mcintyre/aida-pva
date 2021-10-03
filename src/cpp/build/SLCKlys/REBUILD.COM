$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCKLYS]
$ @BUILD
$ cd [---.lib.SLCKLYS]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
