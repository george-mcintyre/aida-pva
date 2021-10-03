$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCBUFFACQ]
$ @BUILD
$ cd [---.lib.SLCBUFFACQ]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
