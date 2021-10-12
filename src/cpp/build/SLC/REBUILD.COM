$! Rebuild and rerun AIDA-PVA provider
$ cd [--.SRC.BUILD.SLC]
$ @BUILD
$ cd [---.lib.SLC]
$ java -jar "-Djava.library.path=." -DAIDA_PVA_CHANNELS_FILENAME=channels.yaml ../aida-pva.jar
