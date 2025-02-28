# 3.1 - Building AIDA-PVA Service

## 3.1.1 Check out code from git repository

On Linux Machine
```shell
git clone git@github.com:slaclab/aida-pva.git
```

## 3.1.2. If you've changed the JNI interface in any way rebuild header file

```shell
cd aida-pva
javah -o src/cpp/slac_aida_NativeChannelProvider.h -classpath ./target/classes slac.aida.NativeChannelProvider
# Manually update C_INC/slac_aida_NativeChannelProvider.h with changes in src/cpp/slac_aida_NativeChannelProvider.h
# Commit to CMS
```

## 3.1.3. Run maven to create jar 
```shell
cd aida-pva
mvn install
[INFO] Scanning for projects...
[INFO] 
[INFO] ------------------< edu.stanford.slac.aida:aida-pva >-------------------
[INFO] Building Aida PVA 3.0.0
[INFO] --------------------------------[ jar ]---------------------------------
Downloading from sonatype-nexus-snapshots: https://oss.sonatype.org/content/repositories/snapshots/org/epics/epics-parent/7.0.8-bp/maven-metadata.xml
Downloaded from sonatype-nexus-snapshots: https://oss.sonatype.org/content/repositories/snapshots/org/epics/epics-parent/7.0.8-bp/maven-metadata.xml (598 B at 576 B/s)
Downloading from sonatype-nexus-snapshots: https://oss.sonatype.org/content/repositories/snapshots/org/epics/epics-pvdata/6.1.7-bp/maven-metadata.xml
Downloaded from sonatype-nexus-snapshots: https://oss.sonatype.org/content/repositories/snapshots/org/epics/epics-pvdata/6.1.7-bp/maven-metadata.xml (1.2 kB at 4.3 kB/s)
Downloading from sonatype-nexus-snapshots: https://oss.sonatype.org/content/repositories/snapshots/org/epics/epics-util/1.0.5-bp/maven-metadata.xml
Downloaded from sonatype-nexus-snapshots: https://oss.sonatype.org/content/repositories/snapshots/org/epics/epics-util/1.0.5-bp/maven-metadata.xml (1.2 kB at 4.3 kB/s)
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ aida-pva ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 1 resource
[INFO] 
[INFO] --- maven-compiler-plugin:3.8.1:compile (default-compile) @ aida-pva ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 29 source files to target/classes
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ aida-pva ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.8.1:testCompile (default-testCompile) @ aida-pva ---
[INFO] Changes detected - recompiling the module!
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ aida-pva ---
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ aida-pva ---
[INFO] Building jar: target/aida-pva-3.0.0.jar
[INFO] 
[INFO] --- maven-shade-plugin:3.2.1:shade (default) @ aida-pva ---
[INFO] Including org.epics:epics-pvaccess:jar:5.1.7-bp in the shaded jar.
[INFO] Including org.epics:jca:jar:2.3.6 in the shaded jar.
[INFO] Including org.apache.felix:maven-bundle-plugin:jar:5.1.1 in the shaded jar.
[INFO] Including org.osgi:org.osgi.core:jar:6.0.0 in the shaded jar.
[INFO] Including biz.aQute.bnd:biz.aQute.bndlib:jar:5.1.1 in the shaded jar.
[INFO] Including org.slf4j:slf4j-api:jar:1.7.25 in the shaded jar.
[INFO] Including org.apache.felix:org.apache.felix.bundlerepository:jar:1.6.6 in the shaded jar.
[INFO] Including org.easymock:easymock:jar:2.4 in the shaded jar.
[INFO] Including org.apache.felix:org.apache.felix.utils:jar:1.6.0 in the shaded jar.
[INFO] Including org.osgi:org.osgi.compendium:jar:4.2.0 in the shaded jar.
[INFO] Including org.apache.maven:maven-core:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-model:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-settings:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-settings-builder:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-builder-support:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-repository-metadata:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-artifact:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-plugin-api:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven:maven-model-builder:jar:3.3.9 in the shaded jar.
[INFO] Including com.google.guava:guava:jar:18.0 in the shaded jar.
[INFO] Including org.apache.maven:maven-aether-provider:jar:3.3.9 in the shaded jar.
[INFO] Including org.eclipse.aether:aether-spi:jar:1.0.2.v20150114 in the shaded jar.
[INFO] Including org.eclipse.aether:aether-impl:jar:1.0.2.v20150114 in the shaded jar.
[INFO] Including org.eclipse.aether:aether-api:jar:1.0.2.v20150114 in the shaded jar.
[INFO] Including org.eclipse.aether:aether-util:jar:1.0.2.v20150114 in the shaded jar.
[INFO] Including org.eclipse.sisu:org.eclipse.sisu.plexus:jar:0.3.2 in the shaded jar.
[INFO] Including javax.enterprise:cdi-api:jar:1.0 in the shaded jar.
[INFO] Including javax.annotation:jsr250-api:jar:1.0 in the shaded jar.
[INFO] Including org.eclipse.sisu:org.eclipse.sisu.inject:jar:0.3.2 in the shaded jar.
[INFO] Including com.google.inject:guice:jar:no_aop:4.0 in the shaded jar.
[INFO] Including javax.inject:javax.inject:jar:1 in the shaded jar.
[INFO] Including aopalliance:aopalliance:jar:1.0 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-interpolation:jar:1.21 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-classworlds:jar:2.5.2 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-component-annotations:jar:1.6 in the shaded jar.
[INFO] Including org.sonatype.plexus:plexus-sec-dispatcher:jar:1.3 in the shaded jar.
[INFO] Including org.sonatype.plexus:plexus-cipher:jar:1.4 in the shaded jar.
[INFO] Including org.apache.commons:commons-lang3:jar:3.4 in the shaded jar.
[INFO] Including org.apache.maven:maven-compat:jar:3.3.9 in the shaded jar.
[INFO] Including org.apache.maven.wagon:wagon-provider-api:jar:2.10 in the shaded jar.
[INFO] Including org.apache.maven.reporting:maven-reporting-api:jar:3.0 in the shaded jar.
[INFO] Including org.apache.maven:maven-archiver:jar:3.5.0 in the shaded jar.
[INFO] Including org.apache.maven.shared:maven-shared-utils:jar:3.2.1 in the shaded jar.
[INFO] Including commons-io:commons-io:jar:2.5 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-archiver:jar:4.2.0 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-io:jar:3.2.0 in the shaded jar.
[INFO] Including org.apache.commons:commons-compress:jar:1.19 in the shaded jar.
[INFO] Including org.iq80.snappy:snappy:jar:0.4 in the shaded jar.
[INFO] Including org.tukaani:xz:jar:1.8 in the shaded jar.
[INFO] Including org.apache.maven.shared:maven-dependency-tree:jar:3.0 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-utils:jar:3.3.0 in the shaded jar.
[INFO] Including org.sonatype.plexus:plexus-build-api:jar:0.0.7 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-sink-api:jar:1.0 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-site-renderer:jar:1.0 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-core:jar:1.0 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-i18n:jar:1.0-beta-7 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-container-default:jar:1.0-alpha-30 in the shaded jar.
[INFO] Including org.codehaus.plexus:plexus-velocity:jar:1.1.7 in the shaded jar.
[INFO] Including org.apache.velocity:velocity:jar:1.5 in the shaded jar.
[INFO] Including commons-lang:commons-lang:jar:2.1 in the shaded jar.
[INFO] Including oro:oro:jar:2.0.8 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-decoration-model:jar:1.0 in the shaded jar.
[INFO] Including commons-collections:commons-collections:jar:3.2 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-module-apt:jar:1.0 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-module-fml:jar:1.0 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-module-xdoc:jar:1.0 in the shaded jar.
[INFO] Including org.apache.maven.doxia:doxia-module-xhtml:jar:1.0 in the shaded jar.
[INFO] Including org.jdom:jdom:jar:1.1 in the shaded jar.
[INFO] Including org.epics:epics-pvdata:jar:6.1.7-bp in the shaded jar.
[INFO] Including org.epics:epics-util:jar:1.0.5-bp in the shaded jar.
[INFO] Including joda-time:joda-time:jar:2.10.10 in the shaded jar.
[INFO] Including org.epics:epics-ntypes:jar:0.3.7-bp in the shaded jar.
[INFO] Including com.fasterxml.jackson.core:jackson-databind:jar:2.1.5 in the shaded jar.
[INFO] Including com.fasterxml.jackson.core:jackson-annotations:jar:2.1.5 in the shaded jar.
[INFO] Including com.fasterxml.jackson.core:jackson-core:jar:2.1.5 in the shaded jar.
[INFO] Including com.fasterxml.jackson.dataformat:jackson-dataformat-yaml:jar:2.1.6-SNAPSHOT in the shaded jar.
[INFO] Including org.json:json:jar:20210307 in the shaded jar.
[WARNING] biz.aQute.bndlib-5.1.1.jar, org.osgi.compendium-4.2.0.jar define 4 overlapping classes: 
[WARNING]   - org.osgi.service.log.LogListener
[WARNING]   - org.osgi.service.log.LogReaderService
[WARNING]   - org.osgi.service.log.LogEntry
[WARNING]   - org.osgi.service.log.LogService
[WARNING] biz.aQute.bndlib-5.1.1.jar, org.osgi.core-6.0.0.jar define 69 overlapping classes: 
[WARNING]   - org.osgi.framework.AdminPermission$1
[WARNING]   - org.osgi.framework.FrameworkUtil$FilterImpl$Parser
[WARNING]   - org.osgi.framework.PrototypeServiceFactory
[WARNING]   - org.osgi.framework.Bundle
[WARNING]   - org.osgi.framework.FrameworkUtil$DNChainMatching
[WARNING]   - org.osgi.framework.launch.FrameworkFactory
[WARNING]   - org.osgi.framework.PackagePermission
[WARNING]   - org.osgi.resource.Wiring
[WARNING]   - org.osgi.framework.AdaptPermissionCollection
[WARNING]   - org.osgi.framework.VersionRange
[WARNING]   - 59 more...
[WARNING] biz.aQute.bndlib-5.1.1.jar, org.osgi.core-6.0.0.jar, org.osgi.compendium-4.2.0.jar define 8 overlapping classes: 
[WARNING]   - org.osgi.util.tracker.ServiceTrackerCustomizer
[WARNING]   - org.osgi.util.tracker.ServiceTracker
[WARNING]   - org.osgi.util.tracker.BundleTracker$Tracked
[WARNING]   - org.osgi.util.tracker.BundleTracker
[WARNING]   - org.osgi.util.tracker.BundleTrackerCustomizer
[WARNING]   - org.osgi.util.tracker.ServiceTracker$AllTracked
[WARNING]   - org.osgi.util.tracker.AbstractTracked
[WARNING]   - org.osgi.util.tracker.ServiceTracker$Tracked
[WARNING] org.apache.felix.bundlerepository-1.6.6.jar, org.apache.felix.utils-1.6.0.jar define 24 overlapping classes: 
[WARNING]   - org.apache.felix.utils.version.VersionCleaner
[WARNING]   - org.apache.felix.utils.filter.FilterImpl
[WARNING]   - org.apache.felix.utils.collections.DictionaryAsMap$KeyEntry
[WARNING]   - org.apache.felix.utils.filter.FilterImpl$ServiceReferenceDictionary
[WARNING]   - org.apache.felix.utils.filter.FilterImpl$SetAccessibleAction
[WARNING]   - org.apache.felix.utils.properties.Properties$PropertiesReader
[WARNING]   - org.apache.felix.utils.collections.DictionaryAsMap$1
[WARNING]   - org.apache.felix.utils.version.VersionRange
[WARNING]   - org.apache.felix.utils.manifest.Parser
[WARNING]   - org.apache.felix.utils.collections.DictionaryAsMap$1$1
[WARNING]   - 14 more...
[WARNING] org.eclipse.sisu.plexus-0.3.2.jar, plexus-container-default-1.0-alpha-30.jar define 82 overlapping classes: 
[WARNING]   - org.codehaus.plexus.component.configurator.MapOrientedComponentConfigurator
[WARNING]   - org.codehaus.plexus.configuration.PlexusConfigurationException
[WARNING]   - org.codehaus.plexus.component.configurator.ComponentConfigurator
[WARNING]   - org.codehaus.plexus.component.repository.ComponentRequirement
[WARNING]   - org.codehaus.plexus.component.factory.AbstractComponentFactory
[WARNING]   - org.codehaus.plexus.PlexusTestCase
[WARNING]   - org.codehaus.plexus.logging.Logger
[WARNING]   - org.codehaus.plexus.component.configurator.BasicComponentConfigurator
[WARNING]   - org.codehaus.plexus.personality.plexus.lifecycle.phase.StartingException
[WARNING]   - org.codehaus.plexus.component.configurator.converters.lookup.DefaultConverterLookup
[WARNING]   - 72 more...
[WARNING] maven-compat-3.3.9.jar, maven-reporting-api-3.0.jar define 1 overlapping classes: 
[WARNING]   - org.apache.maven.reporting.MavenReportException
[WARNING] maven-shade-plugin has detected that some class files are
[WARNING] present in two or more JARs. When this happens, only one
[WARNING] single version of the class is copied to the uber jar.
[WARNING] Usually this is not harmful and you can skip these warnings,
[WARNING] otherwise try to manually exclude artifacts based on
[WARNING] mvn dependency:tree -Ddetail=true and the above output.
[WARNING] See http://maven.apache.org/plugins/maven-shade-plugin/
[INFO] Replacing original artifact with shaded artifact.
[INFO] Replacing target/aida-pva-3.0.0.jar with target/aida-pva-3.0.0-shaded.jar
[INFO] Dependency-reduced POM written at: dependency-reduced-pom.xml
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ aida-pva ---
[INFO] Installing target/aida-pva-3.0.0.jar to /.m2/repository/edu/stanford/slac/aida/aida-pva/3.0.0/aida-pva-3.0.0.jar
[INFO] Installing dependency-reduced-pom.xml to /.m2/repository/edu/stanford/slac/aida/aida-pva/3.0.0/aida-pva-3.0.0.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  8.600 s
[INFO] Finished at: 2021-10-31T00:04:02+02:00
[INFO] ------------------------------------------------------------------------

Process finished with exit code 0

```

## 3.1.4. Copy jar to VMS
```shell
cp /.m2/repository/edu/stanford/slac/aida/aida-pva/3.0.0/aida-pva-3.0.0.jar aida-pva.jar
sftp mccdev
Connecting to mccdev...
 
sftp> mput aida-pva.jar 

```

## 3.1.5. Use TESTJAR to test the new JAR file privately

### Summary
- Use   `TESTJAR <list of jar files>`   to supply .JAR file(-s) for testing
- Use   `NOTESTJAR`                     to revert to the standard `JAVA$CLASSPATH` .JAR file list
- Use   `SHOWTESTJAR`                   to display the test `JAVA$CLASSPATH` logical

### More details:

If you want to test a new version of a .JAR file that is already listed in the standard
JOB table logical JAVA$CLASSPATH (or is INTENDED to be listed there) proceed as follows:

   `$ TESTJAR <comma-separated list of .JAR files for testing>`

### Examples:
```shell
$ TESTJAR []MYJAR.JAR
$ TESTJAR [],UDSLC:[USER.DEVJAR]SOMEJAR.JAR;10  ! [] will find any CLASS files in default direc
```

### Details
`TESTJAR` will define a PROCESS table logical name `JAVA$CLASSPATH` by prepending the specified
list to the list of files defined in the standard JOB table `JAVA$CLASSPATH` logical name.
Note that file names do not need to be fully qualified only if you maintain the same default directory
while you do your testing.

Test by running the java procedure that uses the new .JAR file(-s)

When you are through testing, issue the command
```shell
$ NOTESTJAR
```
which simply DEASSIGNs your PROCESS table `JAVA$CLASSPATH` logical
or, alternatively, just `LOGOUT`

If you want to see the current test version of the test `JAVA$CLASSPATH` logical issue the command:
```shell
$ SHOWTESTJAR
```

To back out a .JAR file stored in SLCIMAGE:, see command BACKJAR.

@note If you are testing a new .JAR file that is NOT already listed in the standard JOB table logical
`JAVA$CLASSPATH`, that file must be included in the definition of `JAVA$CLASSPATH` (in file
`SLCCOM:USERLOGIN.COM`) in the form `SLCIMAGE:xxx.JAR` before your new .JAR file is released
to DEV and NEW.

## 3.1.6. Create a Development JAR to deploy on MCCDEV for further testing
Use command `DEVIMAGE` to move a .JAR file (that is to be stored in `SLCIMAGE`) into the DEV directory.

### Example:
```shell
$ DEVIMAGE MYJAR.JAR
```

@note These SLCIMAGE: .JAR files should normally be listed in the logical name
`JAVA$CLASSPATH` (which is defined in file `SLCCOM:USERLOGIN.COM`.)

## 3.1.7. Move JAR to production
Use command `NEWIMAGE` to move a .JAR file (that is stored in `SLCIMAGE`) from DEV to NEW.

### Example:
```shell
$ NEWIMAGE MYJAR.JAR
```

## 3.1.8 To back out a jar stored in SLCIMAGE
Use command `BACKIMAGE` to backout a .JAR file (that is stored in `SLCIMAGE`).

### Example:
```shell
$ BACKIMAGE MYJAR.JAR
```
