$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLCMODEL]
$ @BUILD
$ cd [---.lib.SLCMODEL]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
