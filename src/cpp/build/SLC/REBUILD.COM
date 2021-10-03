$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLC]
$ @BUILD
$ cd [---.lib.SLC]
$ java -jar "-Djava.library.path=." ../aida-pva.jar
