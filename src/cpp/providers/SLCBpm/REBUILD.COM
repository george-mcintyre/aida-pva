$! Rebuild and rerun AIDA-PVA provider
$ @BUILD
$ java -jar "-Djava.library.path=." ../../lib/aida-pva.jar
