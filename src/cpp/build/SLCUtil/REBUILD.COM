$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCUTIL]
$ @BUILD
$ cd [---.lib.SLCUTIL]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
