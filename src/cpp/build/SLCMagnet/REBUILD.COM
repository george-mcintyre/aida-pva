$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCMAGNET]
$ @BUILD
$ cd [---.lib.SLCMAGNET]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
