# 5 - Porting EPICS to Java 1.5 on VMS
The code for the EPICS port is in github at:

- [Trimmed Down EPICS backport: Server Only](https://github.com/slaclab/epics-server-java-backport-1.5)
- [Full EPICS backport](https://github.com/slaclab/epics-core-java-backport-1.5)

To build it: 

1. Check out code from git repository

On Linux Machine
```shell
git clone git@github.com:slaclab/epics-server-java-backport-1.5.git
```

2. Run maven to create jar 
```shell
cd epics-server-java-backport-1.5
mvn install -DskipTests
[INFO] Scanning for projects...
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Build Order:
[INFO] 
[INFO] EPICS Core Server BackPort 1.5 Build                               [pom]
[INFO] org.epics.util                                                     [jar]
[INFO] pvDataJava                                                         [jar]
[INFO] pvAccessJava                                                       [jar]
[INFO] normativeTypesJava                                                 [jar]
[INFO] epics-forwarder                                                    [jar]
[INFO] 
[INFO] -----------------------< org.epics:epics-parent >-----------------------
[INFO] Building EPICS Core Server BackPort 1.5 Build 7.0.8-bp            [1/6]
[INFO] --------------------------------[ pom ]---------------------------------
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ epics-parent ---
[INFO] Installing pom.xml to ~/.m2/repository/org/epics/epics-parent/7.0.8-bp/epics-parent-7.0.8-bp.pom
[INFO] 
[INFO] ------------------------< org.epics:epics-util >------------------------
[INFO] Building org.epics.util 1.0.5-bp                                   [2/6]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ epics-util ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory epics-util/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:compile (default-compile) @ epics-util ---
[INFO] Changes detected - recompiling the module!
[INFO] Compiling 97 source files to epics-util/target/classes
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ epics-util ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory epics-util/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:testCompile (default-testCompile) @ epics-util ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ epics-util ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ epics-util ---
[INFO] Building jar: epics-util/target/epics-util-1.0.5-bp.jar
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ epics-util ---
[INFO] Installing epics-util/target/epics-util-1.0.5-bp.jar to ~/.m2/repository/org/epics/epics-util/1.0.5-bp/epics-util-1.0.5-bp.jar
[INFO] Installing epics-util/pom.xml to ~/.m2/repository/org/epics/epics-util/1.0.5-bp/epics-util-1.0.5-bp.pom
[INFO] 
[INFO] -----------------------< org.epics:epics-pvdata >-----------------------
[INFO] Building pvDataJava 6.1.7-bp                                 [3/6]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ epics-pvdata ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory pvDataJava/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:compile (default-compile) @ epics-pvdata ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ epics-pvdata ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory pvDataJava/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:testCompile (default-testCompile) @ epics-pvdata ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ epics-pvdata ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ epics-pvdata ---
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ epics-pvdata ---
[INFO] Installing pvDataJava/target/epics-pvdata-6.1.7-bp.jar to ~/.m2/repository/org/epics/epics-pvdata/6.1.7-bp/epics-pvdata-6.1.7-bp.jar
[INFO] Installing pvDataJava/pom.xml to ~/.m2/repository/org/epics/epics-pvdata/6.1.7-bp/epics-pvdata-6.1.7-bp.pom
[INFO] 
[INFO] ----------------------< org.epics:epics-pvaccess >----------------------
[INFO] Building pvAccessJava 5.1.7-bp                               [4/6]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ epics-pvaccess ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory pvAccessJava/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:compile (default-compile) @ epics-pvaccess ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ epics-pvaccess ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory pvAccessJava/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:testCompile (default-testCompile) @ epics-pvaccess ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ epics-pvaccess ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ epics-pvaccess ---
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ epics-pvaccess ---
[INFO] Installing pvAccessJava/target/epics-pvaccess-5.1.7-bp.jar to ~/.m2/repository/org/epics/epics-pvaccess/5.1.7-bp/epics-pvaccess-5.1.7-bp.jar
[INFO] Installing pvAccessJava/pom.xml to ~/.m2/repository/org/epics/epics-pvaccess/5.1.7-bp/epics-pvaccess-5.1.7-bp.pom
[INFO] 
[INFO] -----------------------< org.epics:epics-ntypes >-----------------------
[INFO] Building normativeTypesJava 0.3.7-bp                         [5/6]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ epics-ntypes ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory normativeTypesJava/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:compile (default-compile) @ epics-ntypes ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ epics-ntypes ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory normativeTypesJava/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:testCompile (default-testCompile) @ epics-ntypes ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ epics-ntypes ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ epics-ntypes ---
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ epics-ntypes ---
[INFO] Installing normativeTypesJava/target/epics-ntypes-0.3.7-bp.jar to ~/.m2/repository/org/epics/epics-ntypes/0.3.7-bp/epics-ntypes-0.3.7-bp.jar
[INFO] Installing normativeTypesJava/pom.xml to ~/.m2/repository/org/epics/epics-ntypes/0.3.7-bp/epics-ntypes-0.3.7-bp.pom
[INFO] 
[INFO] ---------------------< org.epics:epics-forwarder >----------------------
[INFO] Building epics-forwarder 1.0.0-bp                            [6/6]
[INFO] --------------------------------[ jar ]---------------------------------
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ epics-forwarder ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] Copying 0 resource
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:compile (default-compile) @ epics-forwarder ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ epics-forwarder ---
[INFO] Using 'UTF-8' encoding to copy filtered resources.
[INFO] skip non existing resourceDirectory epics-forwarder/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.5.1:testCompile (default-testCompile) @ epics-forwarder ---
[INFO] Nothing to compile - all classes are up to date
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ epics-forwarder ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-jar-plugin:2.4:jar (default-jar) @ epics-forwarder ---
[INFO] 
[INFO] --- maven-shade-plugin:3.2.1:shade (default) @ epics-forwarder ---
[INFO] Including org.epics:epics-pvaccess:jar:5.1.7-bp in the shaded jar.
[INFO] Including org.epics:epics-pvdata:jar:6.1.7-bp in the shaded jar.
[INFO] Including org.epics:epics-util:jar:1.0.5-bp in the shaded jar.
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
[INFO] Including joda-time:joda-time:jar:2.10.10 in the shaded jar.
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-velocity-1.1.7.jar define 5 overlapping classes: 
[WARNING]   - org.codehaus.plexus.velocity.VelocityComponent
[WARNING]   - org.codehaus.plexus.velocity.VelocityComponent$1
[WARNING]   - org.codehaus.plexus.velocity.ContextClassLoaderResourceLoader
[WARNING]   - org.codehaus.plexus.velocity.SiteResourceLoader
[WARNING]   - org.codehaus.plexus.velocity.DefaultVelocityComponent
[WARNING] plexus-classworlds-2.5.2.jar, epics-forwarder-1.0.0-bp.jar define 37 overlapping classes: 
[WARNING]   - org.codehaus.classworlds.DefaultClassRealm
[WARNING]   - org.codehaus.classworlds.ClassWorldReverseAdapter
[WARNING]   - org.codehaus.plexus.classworlds.strategy.Strategy
[WARNING]   - org.codehaus.classworlds.BytesURLConnection
[WARNING]   - org.codehaus.classworlds.ClassRealmAdapter
[WARNING]   - org.codehaus.plexus.classworlds.ClassWorldException
[WARNING]   - org.codehaus.classworlds.NoSuchRealmException
[WARNING]   - org.codehaus.plexus.classworlds.strategy.AbstractStrategy
[WARNING]   - org.codehaus.classworlds.DuplicateRealmException
[WARNING]   - org.codehaus.classworlds.ClassWorldAdapter
[WARNING]   - 27 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, org.apache.felix.utils-1.6.0.jar define 12 overlapping classes: 
[WARNING]   - org.apache.felix.utils.extender.AbstractExtender$1
[WARNING]   - org.apache.felix.utils.service.BaseManagedServiceFactory$1
[WARNING]   - org.apache.felix.utils.extender.AbstractExtender$2
[WARNING]   - org.apache.felix.utils.extender.SimpleExtension
[WARNING]   - org.apache.felix.utils.extender.Extension
[WARNING]   - org.apache.felix.utils.extender.AbstractExtender
[WARNING]   - org.apache.felix.utils.properties.InterpolationHelper$SubstitutionCallback
[WARNING]   - org.apache.felix.utils.service.BaseManagedServiceFactory
[WARNING]   - org.apache.felix.utils.properties.Properties$1
[WARNING]   - org.apache.felix.utils.service.BaseManagedServiceFactory$2
[WARNING]   - 2 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-sec-dispatcher-1.3.jar define 13 overlapping classes: 
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.DefaultSecDispatcher
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.PasswordDecryptorException
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.PasswordDecryptor$1
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.model.io.xpp3.SecurityConfigurationXpp3Reader
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.SecUtil
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.model.SettingsSecurity
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.model.Config
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.SecDispatcherException
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.model.io.xpp3.SecurityConfigurationXpp3Writer
[WARNING]   - org.sonatype.plexus.components.sec.dispatcher.PasswordDecryptor
[WARNING]   - 3 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-cipher-1.4.jar define 6 overlapping classes: 
[WARNING]   - org.sonatype.plexus.components.cipher.PlexusCipherException
[WARNING]   - org.sonatype.plexus.components.cipher.PlexusCipher
[WARNING]   - org.sonatype.plexus.components.cipher.Base64
[WARNING]   - org.sonatype.plexus.components.cipher.PlexusCipher$1
[WARNING]   - org.sonatype.plexus.components.cipher.DefaultPlexusCipher
[WARNING]   - org.sonatype.plexus.components.cipher.PBECipher
[WARNING] epics-forwarder-1.0.0-bp.jar, jdom-1.1.jar define 76 overlapping classes: 
[WARNING]   - org.jdom.transform.JDOMResult
[WARNING]   - org.jdom.transform.JDOMSource
[WARNING]   - org.jdom.DataConversionException
[WARNING]   - org.jdom.input.DOMBuilder
[WARNING]   - org.jdom.xpath.JaxenXPath$NSContext
[WARNING]   - org.jdom.output.Format$DefaultEscapeStrategy
[WARNING]   - org.jdom.ContentList$FilterListIterator
[WARNING]   - org.jdom.ProcessingInstruction
[WARNING]   - org.jdom.DocType
[WARNING]   - org.jdom.transform.JDOMResult$DocumentBuilder
[WARNING]   - 66 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, org.eclipse.sisu.plexus-0.3.2.jar define 82 overlapping classes: 
[WARNING]   - org.eclipse.sisu.plexus.RequirementImpl
[WARNING]   - org.eclipse.sisu.plexus.PlexusBundlePlan
[WARNING]   - org.eclipse.sisu.plexus.PlexusAnnotatedMetadata
[WARNING]   - org.codehaus.plexus.component.configurator.converters.basic.UriConverter
[WARNING]   - org.eclipse.sisu.plexus.PlexusBeanLocator
[WARNING]   - org.eclipse.sisu.plexus.PlexusRequirements$AbstractRequirementProvider
[WARNING]   - org.codehaus.plexus.DefaultPlexusContainer$DefaultsModule
[WARNING]   - org.eclipse.sisu.plexus.ComponentDescriptorBeanModule$PlexusDescriptorBeanSource
[WARNING]   - org.eclipse.sisu.plexus.PlexusBindingModule
[WARNING]   - org.eclipse.sisu.plexus.PlexusTypeBinder
[WARNING]   - 72 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, oro-2.0.8.jar define 62 overlapping classes: 
[WARNING]   - org.apache.oro.text.perl.MalformedPerl5PatternException
[WARNING]   - org.apache.oro.text.regex.Perl5Pattern
[WARNING]   - org.apache.oro.text.GenericPatternCache
[WARNING]   - org.apache.oro.util.Cache
[WARNING]   - org.apache.oro.io.RegexFilenameFilter
[WARNING]   - org.apache.oro.text.awk.DFAState
[WARNING]   - org.apache.oro.text.regex.StringSubstitution
[WARNING]   - org.apache.oro.text.awk.AwkCompiler
[WARNING]   - org.apache.oro.text.PatternCacheLRU
[WARNING]   - org.apache.oro.text.regex.Substitution
[WARNING]   - 52 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-interpolation-1.21.jar define 44 overlapping classes: 
[WARNING]   - org.codehaus.plexus.interpolation.PrefixedPropertiesValueSource
[WARNING]   - org.codehaus.plexus.interpolation.multi.MultiDelimiterInterpolatorFilterReader
[WARNING]   - org.codehaus.plexus.interpolation.FeedbackEnabledValueSource
[WARNING]   - org.codehaus.plexus.interpolation.Interpolator
[WARNING]   - org.codehaus.plexus.interpolation.object.FieldBasedObjectInterpolator$1
[WARNING]   - org.codehaus.plexus.interpolation.PrefixedValueSourceWrapper
[WARNING]   - org.codehaus.plexus.interpolation.reflection.MethodMap
[WARNING]   - org.codehaus.plexus.interpolation.InterpolationCycleException
[WARNING]   - org.codehaus.plexus.interpolation.reflection.ReflectionValueExtractor
[WARNING]   - org.codehaus.plexus.interpolation.QueryEnabledValueSource
[WARNING]   - 34 more...
[WARNING] epics-util-1.0.5-bp.jar, epics-forwarder-1.0.0-bp.jar define 161 overlapping classes: 
[WARNING]   - org.epics.util.compat.jdk5.functional.Consumer
[WARNING]   - org.epics.util.array.ListDouble
[WARNING]   - org.epics.util.array.UnsafeUnwrapper$Array
[WARNING]   - org.epics.util.array.ListULong
[WARNING]   - org.epics.util.stats.Ranges
[WARNING]   - org.epics.util.array.ListUShort$1
[WARNING]   - org.epics.util.concurrent.Executors$1
[WARNING]   - org.epics.util.array.ListShort$2
[WARNING]   - org.epics.util.array.ArrayFloat$1
[WARNING]   - org.epics.util.stats.Statistics
[WARNING]   - 151 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-archiver-4.2.0.jar define 132 overlapping classes: 
[WARNING]   - org.codehaus.plexus.archiver.tar.XZTarFile
[WARNING]   - org.codehaus.plexus.archiver.tar.PlexusIoTarSnappyFileResourceCollection
[WARNING]   - org.codehaus.plexus.archiver.zip.ZipSymlinkResource
[WARNING]   - org.codehaus.plexus.archiver.jar.Manifest$Attribute
[WARNING]   - org.codehaus.plexus.archiver.tar.TarArchiver
[WARNING]   - org.codehaus.plexus.archiver.FinalizerEnabled
[WARNING]   - org.codehaus.plexus.archiver.filters.JarSecurityFileFilter
[WARNING]   - org.codehaus.plexus.archiver.zip.ConcurrentJarCreator
[WARNING]   - org.codehaus.plexus.archiver.dir.DirectoryArchiver
[WARNING]   - org.codehaus.plexus.archiver.tar.TarSnappyUnArchiver
[WARNING]   - 122 more...
[WARNING] maven-plugin-api-3.3.9.jar, epics-forwarder-1.0.0-bp.jar define 27 overlapping classes: 
[WARNING]   - org.apache.maven.plugin.MojoFailureException
[WARNING]   - org.apache.maven.plugin.descriptor.Parameter
[WARNING]   - org.apache.maven.monitor.logging.DefaultLog
[WARNING]   - org.apache.maven.plugin.lifecycle.Phase
[WARNING]   - org.apache.maven.plugin.lifecycle.io.xpp3.LifecycleMappingsXpp3Writer
[WARNING]   - org.apache.maven.plugin.descriptor.PluginDescriptor
[WARNING]   - org.apache.maven.plugin.lifecycle.Lifecycle
[WARNING]   - org.apache.maven.plugin.lifecycle.io.xpp3.LifecycleMappingsXpp3Reader
[WARNING]   - org.apache.maven.plugin.descriptor.InvalidParameterException
[WARNING]   - org.apache.maven.plugin.MojoExecutionException
[WARNING]   - 17 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, guava-18.0.jar define 1690 overlapping classes: 
[WARNING]   - com.google.common.collect.ImmutableMapValues$1
[WARNING]   - com.google.common.io.LineProcessor
[WARNING]   - com.google.common.util.concurrent.AbstractService$5
[WARNING]   - com.google.common.io.BaseEncoding$StandardBaseEncoding$2
[WARNING]   - com.google.common.io.ByteProcessor
[WARNING]   - com.google.common.math.package-info
[WARNING]   - com.google.common.util.concurrent.SimpleTimeLimiter
[WARNING]   - com.google.common.io.GwtWorkarounds$5
[WARNING]   - com.google.common.cache.AbstractCache$StatsCounter
[WARNING]   - com.google.common.util.concurrent.CycleDetectingLockFactory$Policies
[WARNING]   - 1680 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, cdi-api-1.0.jar define 75 overlapping classes: 
[WARNING]   - javax.enterprise.context.ApplicationScoped
[WARNING]   - javax.enterprise.inject.InjectionException
[WARNING]   - javax.enterprise.context.SessionScoped
[WARNING]   - javax.enterprise.inject.IllegalProductException
[WARNING]   - javax.enterprise.inject.Disposes
[WARNING]   - javax.enterprise.inject.Stereotype
[WARNING]   - javax.enterprise.util.TypeLiteral
[WARNING]   - javax.enterprise.inject.spi.BeanManager
[WARNING]   - javax.enterprise.inject.spi.BeforeBeanDiscovery
[WARNING]   - javax.enterprise.inject.Model
[WARNING]   - 65 more...
[WARNING] wagon-provider-api-2.10.jar, epics-forwarder-1.0.0-bp.jar define 39 overlapping classes: 
[WARNING]   - org.apache.maven.wagon.events.SessionEvent
[WARNING]   - org.apache.maven.wagon.proxy.ProxyInfoProvider
[WARNING]   - org.apache.maven.wagon.observers.AbstractTransferListener
[WARNING]   - org.apache.maven.wagon.authorization.AuthorizationException
[WARNING]   - org.apache.maven.wagon.events.TransferEventSupport
[WARNING]   - org.apache.maven.wagon.Wagon
[WARNING]   - org.apache.maven.wagon.observers.Debug
[WARNING]   - org.apache.maven.wagon.events.TransferListener
[WARNING]   - org.apache.maven.wagon.PathUtils
[WARNING]   - org.apache.maven.wagon.LazyFileOutputStream
[WARNING]   - 29 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-compat-3.3.9.jar define 162 overlapping classes: 
[WARNING]   - org.apache.maven.repository.MetadataResolutionResult
[WARNING]   - org.apache.maven.repository.legacy.resolver.conflict.NearestConflictResolver
[WARNING]   - org.apache.maven.repository.metadata.DefaultClasspathTransformation$ClasspathGraphVisitor$1
[WARNING]   - org.apache.maven.artifact.installer.ArtifactInstaller
[WARNING]   - org.apache.maven.artifact.resolver.DefaultArtifactCollector
[WARNING]   - org.apache.maven.artifact.resolver.ResolutionListenerForDepMgmt
[WARNING]   - org.apache.maven.repository.legacy.TransferListenerAdapter
[WARNING]   - org.apache.maven.profiles.activation.FileProfileActivator
[WARNING]   - org.apache.maven.artifact.repository.metadata.RepositoryMetadataReadException
[WARNING]   - org.apache.maven.artifact.repository.metadata.MetadataBridge
[WARNING]   - 152 more...
[WARNING] easymock-2.4.jar, epics-forwarder-1.0.0-bp.jar define 71 overlapping classes: 
[WARNING]   - org.easymock.internal.matchers.LessOrEqual
[WARNING]   - org.easymock.internal.ExpectedInvocation
[WARNING]   - org.easymock.internal.matchers.Any
[WARNING]   - org.easymock.LogicalOperator$3
[WARNING]   - org.easymock.internal.MocksBehavior
[WARNING]   - org.easymock.Capture
[WARNING]   - org.easymock.internal.ExpectedInvocationAndResult
[WARNING]   - org.easymock.internal.EqualsMatcher
[WARNING]   - org.easymock.internal.matchers.CompareEqual
[WARNING]   - org.easymock.IArgumentMatcher
[WARNING]   - 61 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-builder-support-3.3.9.jar define 10 overlapping classes: 
[WARNING]   - org.apache.maven.building.ProblemCollector
[WARNING]   - org.apache.maven.building.UrlSource
[WARNING]   - org.apache.maven.building.DefaultProblemCollector
[WARNING]   - org.apache.maven.building.FileSource
[WARNING]   - org.apache.maven.building.Source
[WARNING]   - org.apache.maven.building.ProblemCollectorFactory
[WARNING]   - org.apache.maven.building.Problem
[WARNING]   - org.apache.maven.building.DefaultProblem
[WARNING]   - org.apache.maven.building.StringSource
[WARNING]   - org.apache.maven.building.Problem$Severity
[WARNING] maven-artifact-3.3.9.jar, epics-forwarder-1.0.0-bp.jar define 32 overlapping classes: 
[WARNING]   - org.apache.maven.repository.Proxy
[WARNING]   - org.apache.maven.artifact.resolver.filter.ArtifactFilter
[WARNING]   - org.apache.maven.artifact.DefaultArtifact
[WARNING]   - org.apache.maven.artifact.versioning.VersionRange
[WARNING]   - org.apache.maven.artifact.metadata.ArtifactMetadata
[WARNING]   - org.apache.maven.artifact.versioning.ComparableVersion$Item
[WARNING]   - org.apache.maven.repository.legacy.metadata.ArtifactMetadata
[WARNING]   - org.apache.maven.artifact.versioning.ComparableVersion$ListItem
[WARNING]   - org.apache.maven.artifact.versioning.ComparableVersion$IntegerItem
[WARNING]   - org.apache.maven.artifact.resolver.AbstractArtifactResolutionException
[WARNING]   - 22 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, commons-io-2.5.jar define 123 overlapping classes: 
[WARNING]   - org.apache.commons.io.FileCleaningTracker
[WARNING]   - org.apache.commons.io.comparator.SizeFileComparator
[WARNING]   - org.apache.commons.io.input.CloseShieldInputStream
[WARNING]   - org.apache.commons.io.filefilter.EmptyFileFilter
[WARNING]   - org.apache.commons.io.monitor.FileEntry
[WARNING]   - org.apache.commons.io.output.ThresholdingOutputStream
[WARNING]   - org.apache.commons.io.input.TailerListener
[WARNING]   - org.apache.commons.io.IOExceptionWithCause
[WARNING]   - org.apache.commons.io.comparator.PathFileComparator
[WARNING]   - org.apache.commons.io.filefilter.NotFileFilter
[WARNING]   - 113 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, doxia-core-1.0.jar define 48 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.util.LineBreaker
[WARNING]   - org.apache.maven.doxia.sink.AbstractTextSink
[WARNING]   - org.apache.maven.doxia.markup.XmlMarkup
[WARNING]   - org.apache.maven.doxia.macro.snippet.SnippetReader
[WARNING]   - org.apache.maven.doxia.markup.TextMarkup
[WARNING]   - org.apache.maven.doxia.macro.SwfMacro
[WARNING]   - org.apache.maven.doxia.DefaultDoxia
[WARNING]   - org.apache.maven.doxia.sink.AbstractXmlSink
[WARNING]   - org.apache.maven.doxia.parser.manager.DefaultParserManager
[WARNING]   - org.apache.maven.doxia.module.site.manager.SiteModuleManager$1
[WARNING]   - 38 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-io-3.2.0.jar define 64 overlapping classes: 
[WARNING]   - org.codehaus.plexus.components.io.resources.PlexusIoCompressedFileResourceCollection$2
[WARNING]   - org.codehaus.plexus.components.io.resources.AbstractPlexusIoResource
[WARNING]   - org.codehaus.plexus.components.io.resources.AbstractPlexusIoArchiveResourceCollection
[WARNING]   - org.codehaus.plexus.components.io.functions.InputStreamTransformer
[WARNING]   - org.codehaus.plexus.components.io.resources.PlexusIoFileResource
[WARNING]   - org.codehaus.plexus.components.io.filemappers.IdentityMapper
[WARNING]   - org.codehaus.plexus.components.io.resources.PlexusIoFileResource$2
[WARNING]   - org.codehaus.plexus.components.io.resources.proxy.PlexusIoProxyResourceCollection$FwdIterator
[WARNING]   - org.codehaus.plexus.components.io.resources.PlexusIoFileResourceCollection$1$1
[WARNING]   - org.codehaus.plexus.components.io.resources.Deferred
[WARNING]   - 54 more...
[WARNING] epics-pvaccess-5.1.7-bp.jar, epics-forwarder-1.0.0-bp.jar define 261 overlapping classes: 
[WARNING]   - org.epics.pvaccess.server.rpc.impl.RPCChannelProvider$1
[WARNING]   - org.epics.pvaccess.impl.remote.request.ResponseHandler
[WARNING]   - org.epics.pvaccess.client.ChannelPutRequester
[WARNING]   - org.epics.pvaccess.server.impl.remote.handlers.NoopResponse
[WARNING]   - org.epics.pvaccess.client.ChannelRequest
[WARNING]   - org.epics.pvaccess.server.impl.remote.handlers.GetHandler
[WARNING]   - org.epics.pvaccess.plugins.SecurityPlugin
[WARNING]   - org.epics.pvaccess.server.impl.remote.handlers.BadResponse
[WARNING]   - org.epics.pvaccess.client.impl.remote.ChannelProcessRequestImpl
[WARNING]   - org.epics.pvaccess.client.impl.remote.handlers.AbstractClientResponseHandler
[WARNING]   - 251 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-model-3.3.9.jar define 54 overlapping classes: 
[WARNING]   - org.apache.maven.model.DistributionManagement
[WARNING]   - org.apache.maven.model.ActivationProperty
[WARNING]   - org.apache.maven.model.io.xpp3.MavenXpp3Reader$1
[WARNING]   - org.apache.maven.model.MailingList
[WARNING]   - org.apache.maven.model.Parent
[WARNING]   - org.apache.maven.model.ModelBase
[WARNING]   - org.apache.maven.model.InputLocation
[WARNING]   - org.apache.maven.model.Notifier
[WARNING]   - org.apache.maven.model.DependencyManagement
[WARNING]   - org.apache.maven.model.PluginContainer
[WARNING]   - 44 more...
[WARNING] plexus-i18n-1.0-beta-7.jar, epics-forwarder-1.0.0-bp.jar define 6 overlapping classes: 
[WARNING]   - org.codehaus.plexus.i18n.I18NTokenizer$AcceptLanguage
[WARNING]   - org.codehaus.plexus.i18n.I18N$1
[WARNING]   - org.codehaus.plexus.i18n.I18NTokenizer$1
[WARNING]   - org.codehaus.plexus.i18n.DefaultI18N
[WARNING]   - org.codehaus.plexus.i18n.I18N
[WARNING]   - org.codehaus.plexus.i18n.I18NTokenizer
[WARNING] epics-forwarder-1.0.0-bp.jar, doxia-site-renderer-1.0.jar define 9 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.siterenderer.DefaultSiteRenderer
[WARNING]   - org.apache.maven.doxia.siterenderer.RendererException
[WARNING]   - org.apache.maven.doxia.siterenderer.SiteRenderingContext
[WARNING]   - org.apache.maven.doxia.siterenderer.Renderer
[WARNING]   - org.apache.maven.doxia.siterenderer.DocumentRenderer
[WARNING]   - org.apache.maven.doxia.siterenderer.ModuleReference
[WARNING]   - org.apache.maven.doxia.siterenderer.DoxiaDocumentRenderer
[WARNING]   - org.apache.maven.doxia.siterenderer.Renderer$1
[WARNING]   - org.apache.maven.doxia.siterenderer.sink.SiteRendererSink
[WARNING] epics-forwarder-1.0.0-bp.jar, doxia-module-xdoc-1.0.jar define 13 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocSiteModule
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup$4
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup$5
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup$6
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocSink
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup
[WARNING]   - org.apache.maven.doxia.module.xdoc.XmlWriterXdocSink
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup$1
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup$2
[WARNING]   - org.apache.maven.doxia.module.xdoc.XdocMarkup$7
[WARNING]   - 3 more...
[WARNING] doxia-module-xhtml-1.0.jar, epics-forwarder-1.0.0-bp.jar define 7 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.module.xhtml.XhtmlMarkup$1
[WARNING]   - org.apache.maven.doxia.module.xhtml.decoration.render.RenderingContext
[WARNING]   - org.apache.maven.doxia.module.xhtml.XhtmlParser
[WARNING]   - org.apache.maven.doxia.module.xhtml.AbstractXhtmlSink
[WARNING]   - org.apache.maven.doxia.module.xhtml.XhtmlSink
[WARNING]   - org.apache.maven.doxia.module.xhtml.XhtmlMarkup
[WARNING]   - org.apache.maven.doxia.module.xhtml.StringsMap
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-model-builder-3.3.9.jar define 121 overlapping classes: 
[WARNING]   - org.apache.maven.model.building.DefaultModelBuilder
[WARNING]   - org.apache.maven.model.building.DefaultModelBuilderFactory$StubLifecycleBindingsInjector
[WARNING]   - org.apache.maven.model.building.ModelCacheTag
[WARNING]   - org.apache.maven.model.management.PluginManagementInjector
[WARNING]   - org.apache.maven.model.merge.MavenModelMerger
[WARNING]   - org.apache.maven.model.interpolation.StringSearchModelInterpolator$InterpolateObjectAction$CacheItem
[WARNING]   - org.apache.maven.model.management.DependencyManagementInjector
[WARNING]   - org.apache.maven.model.interpolation.ModelInterpolator
[WARNING]   - org.apache.maven.model.profile.ProfileActivationContext
[WARNING]   - org.apache.maven.model.building.DefaultModelProblemCollector
[WARNING]   - 111 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-bundle-plugin-5.1.1.jar define 59 overlapping classes: 
[WARNING]   - org.apache.felix.obrplugin.ObrUpdate
[WARNING]   - org.apache.maven.shared.osgi.DefaultMaven2OsgiConverter
[WARNING]   - org.apache.felix.bundleplugin.AbstractDependencyFilter$8
[WARNING]   - org.apache.felix.bundleplugin.AbstractDependencyFilter
[WARNING]   - org.apache.felix.obrplugin.RemoteFileManager
[WARNING]   - org.apache.felix.bundleplugin.DependencyEmbedder
[WARNING]   - org.apache.felix.obrplugin.ObrInstall
[WARNING]   - org.apache.felix.bundleplugin.AbstractDependencyFilter$1
[WARNING]   - org.apache.felix.bundleplugin.AntPlugin
[WARNING]   - org.apache.felix.obrplugin.ObrCleanRepo
[WARNING]   - 49 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-archiver-3.5.0.jar define 5 overlapping classes: 
[WARNING]   - org.apache.maven.archiver.PomPropertiesUtil
[WARNING]   - org.apache.maven.archiver.MavenArchiver
[WARNING]   - org.apache.maven.archiver.ManifestSection
[WARNING]   - org.apache.maven.archiver.ManifestConfiguration
[WARNING]   - org.apache.maven.archiver.MavenArchiveConfiguration
[WARNING] jsr250-api-1.0.jar, epics-forwarder-1.0.0-bp.jar define 11 overlapping classes: 
[WARNING]   - javax.annotation.PreDestroy
[WARNING]   - javax.annotation.Resource$AuthenticationType
[WARNING]   - javax.annotation.Generated
[WARNING]   - javax.annotation.security.DeclareRoles
[WARNING]   - javax.annotation.Resource
[WARNING]   - javax.annotation.security.DenyAll
[WARNING]   - javax.annotation.security.RunAs
[WARNING]   - javax.annotation.security.PermitAll
[WARNING]   - javax.annotation.Resources
[WARNING]   - javax.annotation.security.RolesAllowed
[WARNING]   - 1 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, guice-4.0-no_aop.jar define 329 overlapping classes: 
[WARNING]   - com.google.inject.Scope
[WARNING]   - com.google.inject.internal.InjectorImpl$2$1
[WARNING]   - com.google.inject.Binding
[WARNING]   - com.google.inject.spi.TypeConverter
[WARNING]   - com.google.inject.internal.ConstructionProxy
[WARNING]   - com.google.inject.internal.InjectorShell$RootModule
[WARNING]   - com.google.inject.spi.InjectionPoint
[WARNING]   - com.google.inject.spi.StaticInjectionRequest
[WARNING]   - com.google.inject.internal.CycleDetectingLock
[WARNING]   - com.google.inject.internal.AbstractBindingProcessor
[WARNING]   - 319 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, aether-util-1.0.2.v20150114.jar define 101 overlapping classes: 
[WARNING]   - org.eclipse.aether.util.graph.transformer.ConflictResolver$OptionalitySelector
[WARNING]   - org.eclipse.aether.util.version.UnionVersionRange
[WARNING]   - org.eclipse.aether.util.repository.JreProxySelector
[WARNING]   - org.eclipse.aether.util.graph.selector.StaticDependencySelector
[WARNING]   - org.eclipse.aether.util.StringUtils
[WARNING]   - org.eclipse.aether.util.graph.version.HighestVersionFilter
[WARNING]   - org.eclipse.aether.util.repository.ConservativeProxySelector
[WARNING]   - org.eclipse.aether.util.graph.transformer.ConflictResolver$State
[WARNING]   - org.eclipse.aether.util.repository.DefaultProxySelector$ProxyDef
[WARNING]   - org.eclipse.aether.util.filter.PatternExclusionsDependencyFilter
[WARNING]   - 91 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, doxia-module-fml-1.0.jar define 5 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.module.fml.FmlSiteModule
[WARNING]   - org.apache.maven.doxia.module.fml.model.Faqs
[WARNING]   - org.apache.maven.doxia.module.fml.model.Faq
[WARNING]   - org.apache.maven.doxia.module.fml.FmlParser
[WARNING]   - org.apache.maven.doxia.module.fml.model.Part
[WARNING] epics-forwarder-1.0.0-bp.jar, doxia-module-apt-1.0.jar define 27 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.module.apt.AptReaderSource
[WARNING]   - org.apache.maven.doxia.module.apt.AptParser$Verbatim
[WARNING]   - org.apache.maven.doxia.module.apt.AptSource
[WARNING]   - org.apache.maven.doxia.module.apt.AptParser
[WARNING]   - org.apache.maven.doxia.module.apt.AptParser$Section3
[WARNING]   - org.apache.maven.doxia.module.apt.AptSiteModule
[WARNING]   - org.apache.maven.doxia.module.apt.AptParser$PageBreak
[WARNING]   - org.apache.maven.doxia.module.apt.AptMarkup
[WARNING]   - org.apache.maven.doxia.module.apt.AptParser$Block
[WARNING]   - org.apache.maven.doxia.module.apt.AptParser$Paragraph
[WARNING]   - 17 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, org.osgi.core-6.0.0.jar define 81 overlapping classes: 
[WARNING]   - org.osgi.framework.namespace.PackageNamespace
[WARNING]   - org.osgi.framework.dto.ServiceReferenceDTO
[WARNING]   - org.osgi.framework.hooks.bundle.CollisionHook
[WARNING]   - org.osgi.service.condpermadmin.package-info
[WARNING]   - org.osgi.resource.dto.package-info
[WARNING]   - org.osgi.resource.dto.RequirementDTO
[WARNING]   - org.osgi.framework.wiring.package-info
[WARNING]   - org.osgi.service.permissionadmin.PermissionInfo
[WARNING]   - org.osgi.framework.hooks.service.EventHook
[WARNING]   - org.osgi.framework.hooks.bundle.package-info
[WARNING]   - 71 more...
[WARNING] slf4j-api-1.7.25.jar, epics-forwarder-1.0.0-bp.jar define 34 overlapping classes: 
[WARNING]   - org.slf4j.helpers.SubstituteLogger
[WARNING]   - org.slf4j.helpers.NamedLoggerBase
[WARNING]   - org.slf4j.helpers.NOPMDCAdapter
[WARNING]   - org.slf4j.MarkerFactory
[WARNING]   - org.slf4j.helpers.BasicMarker
[WARNING]   - org.slf4j.spi.LoggerFactoryBinder
[WARNING]   - org.slf4j.MDC$MDCCloseable
[WARNING]   - org.slf4j.spi.LocationAwareLogger
[WARNING]   - org.slf4j.helpers.MessageFormatter
[WARNING]   - org.slf4j.helpers.Util$ClassContextSecurityManager
[WARNING]   - 24 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-aether-provider-3.3.9.jar define 26 overlapping classes: 
[WARNING]   - org.apache.maven.repository.internal.DefaultVersionResolver$VersionInfo
[WARNING]   - org.apache.maven.repository.internal.DefaultArtifactDescriptorReader
[WARNING]   - org.apache.maven.repository.internal.MavenAetherModule
[WARNING]   - org.apache.maven.repository.internal.ArtifactDescriptorReaderDelegate
[WARNING]   - org.apache.maven.repository.internal.DefaultModelResolver$1
[WARNING]   - org.apache.maven.repository.internal.RemoteSnapshotMetadataGenerator
[WARNING]   - org.apache.maven.repository.internal.LocalSnapshotMetadataGenerator
[WARNING]   - org.apache.maven.repository.internal.MavenWorkspaceReader
[WARNING]   - org.apache.maven.repository.internal.DefaultVersionResolver$Record
[WARNING]   - org.apache.maven.repository.internal.DefaultModelCache
[WARNING]   - 16 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, aether-spi-1.0.2.v20150114.jar define 34 overlapping classes: 
[WARNING]   - org.eclipse.aether.spi.connector.ArtifactDownload
[WARNING]   - org.eclipse.aether.spi.connector.transport.TransporterProvider
[WARNING]   - org.eclipse.aether.spi.connector.RepositoryConnector
[WARNING]   - org.eclipse.aether.spi.connector.transport.TransporterFactory
[WARNING]   - org.eclipse.aether.spi.log.LoggerFactory
[WARNING]   - org.eclipse.aether.spi.connector.transport.AbstractTransporter
[WARNING]   - org.eclipse.aether.spi.connector.layout.RepositoryLayout
[WARNING]   - org.eclipse.aether.spi.locator.Service
[WARNING]   - org.eclipse.aether.spi.connector.MetadataTransfer
[WARNING]   - org.eclipse.aether.spi.connector.Transfer
[WARNING]   - 24 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, snappy-0.4.jar define 24 overlapping classes: 
[WARNING]   - org.iq80.snappy.HadoopSnappyCodec$SnappyCompressionInputStream
[WARNING]   - org.iq80.snappy.SnappyFramedOutputStream
[WARNING]   - org.iq80.snappy.SnappyInternalUtils
[WARNING]   - org.iq80.snappy.Main
[WARNING]   - org.iq80.snappy.SnappyInputStream
[WARNING]   - org.iq80.snappy.AbstractSnappyInputStream
[WARNING]   - org.iq80.snappy.Snappy
[WARNING]   - org.iq80.snappy.CorruptionException
[WARNING]   - org.iq80.snappy.HadoopSnappyCodec$SnappyCompressionOutputStream
[WARNING]   - org.iq80.snappy.UnsafeMemory
[WARNING]   - 14 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, doxia-sink-api-1.0.jar define 4 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.sink.SinkFactory
[WARNING]   - org.codehaus.doxia.sink.Sink
[WARNING]   - org.apache.maven.doxia.sink.Sink
[WARNING]   - org.apache.maven.doxia.sink.Sink$1
[WARNING] epics-forwarder-1.0.0-bp.jar, aether-impl-1.0.2.v20150114.jar define 96 overlapping classes: 
[WARNING]   - org.eclipse.aether.impl.RepositoryConnectorProvider
[WARNING]   - org.eclipse.aether.internal.impl.Utils
[WARNING]   - org.eclipse.aether.impl.ArtifactDescriptorReader
[WARNING]   - org.eclipse.aether.internal.impl.NodeStack
[WARNING]   - org.eclipse.aether.internal.impl.ArtifactrequestBuilder
[WARNING]   - org.eclipse.aether.internal.impl.DefaultSyncContextFactory
[WARNING]   - org.eclipse.aether.impl.MetadataGenerator
[WARNING]   - org.eclipse.aether.internal.impl.DefaultArtifactResolver$ResolutionItem
[WARNING]   - org.eclipse.aether.internal.impl.DefaultDeployer$EventCatapult
[WARNING]   - org.eclipse.aether.internal.impl.PrioritizedComponents
[WARNING]   - 86 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, jca-2.3.6.jar define 196 overlapping classes: 
[WARNING]   - gov.aps.jca.event.PutListener
[WARNING]   - gov.aps.jca.jni.ThreadSafeContext$MessageCallbackRequest
[WARNING]   - gov.aps.jca.Monitor
[WARNING]   - gov.aps.jca.jni.ThreadSafeContext$PollRequest
[WARNING]   - gov.aps.jca.dbr.INT
[WARNING]   - gov.aps.jca.jni.ThreadSafeContext$AccessRightsRequest
[WARNING]   - gov.aps.jca.event.QueuedEventDispatcher$4
[WARNING]   - gov.aps.jca.dbr.DBR_Float
[WARNING]   - gov.aps.jca.event.LatestMonitorOnlyQueuedEventDispatcher$6
[WARNING]   - gov.aps.jca.jni.ThreadSafeContext$ConnectionCallbackRequest
[WARNING]   - 186 more...
[WARNING] org.apache.felix.bundlerepository-1.6.6.jar, epics-forwarder-1.0.0-bp.jar, org.apache.felix.utils-1.6.0.jar define 24 overlapping classes: 
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
[WARNING] plexus-build-api-0.0.7.jar, epics-forwarder-1.0.0-bp.jar define 4 overlapping classes: 
[WARNING]   - org.sonatype.plexus.build.incremental.BuildContext
[WARNING]   - org.sonatype.plexus.build.incremental.ThreadBuildContext
[WARNING]   - org.sonatype.plexus.build.incremental.EmptyScanner
[WARNING]   - org.sonatype.plexus.build.incremental.DefaultBuildContext
[WARNING] epics-forwarder-1.0.0-bp.jar, joda-time-2.10.10.jar define 247 overlapping classes: 
[WARNING]   - org.joda.time.field.RemainderDateTimeField
[WARNING]   - org.joda.time.convert.ReadableDurationConverter
[WARNING]   - org.joda.time.convert.ReadableIntervalConverter
[WARNING]   - org.joda.time.field.SkipUndoDateTimeField
[WARNING]   - org.joda.time.format.DateTimeFormatterBuilder$TimeZoneName
[WARNING]   - org.joda.time.tz.DateTimeZoneBuilder$RuleSet
[WARNING]   - org.joda.time.chrono.BasicDayOfMonthDateTimeField
[WARNING]   - org.joda.time.Duration
[WARNING]   - org.joda.time.field.OffsetDateTimeField
[WARNING]   - org.joda.time.format.ISODateTimeFormat
[WARNING]   - 237 more...
[WARNING] org.apache.felix.bundlerepository-1.6.6.jar, epics-forwarder-1.0.0-bp.jar define 60 overlapping classes: 
[WARNING]   - org.apache.felix.bundlerepository.impl.Activator
[WARNING]   - org.apache.felix.bundlerepository.impl.RepositoryParser
[WARNING]   - org.apache.felix.bundlerepository.impl.RepositoryAdminImpl$1
[WARNING]   - org.apache.felix.bundlerepository.impl.DataModelHelperImpl$2
[WARNING]   - org.apache.felix.bundlerepository.impl.ObrCommandImpl$1
[WARNING]   - org.apache.felix.bundlerepository.impl.wrapper.ResourceWrapper
[WARNING]   - org.apache.felix.bundlerepository.impl.wrapper.Wrapper
[WARNING]   - org.apache.felix.bundlerepository.impl.ResourceImpl
[WARNING]   - org.apache.felix.bundlerepository.impl.DataModelHelperImpl
[WARNING]   - org.apache.felix.bundlerepository.impl.ResourceCapability
[WARNING]   - 50 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-settings-3.3.9.jar define 19 overlapping classes: 
[WARNING]   - org.apache.maven.settings.RepositoryPolicy
[WARNING]   - org.apache.maven.settings.RepositoryBase
[WARNING]   - org.apache.maven.settings.ActivationFile
[WARNING]   - org.apache.maven.settings.Proxy
[WARNING]   - org.apache.maven.settings.IdentifiableBase
[WARNING]   - org.apache.maven.settings.io.xpp3.SettingsXpp3Reader
[WARNING]   - org.apache.maven.settings.TrackableBase
[WARNING]   - org.apache.maven.settings.RuntimeInfo
[WARNING]   - org.apache.maven.settings.io.xpp3.SettingsXpp3Reader$1
[WARNING]   - org.apache.maven.settings.ActivationOS
[WARNING]   - 9 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-settings-builder-3.3.9.jar define 32 overlapping classes: 
[WARNING]   - org.apache.maven.settings.io.SettingsReader
[WARNING]   - org.apache.maven.settings.building.DefaultSettingsBuildingRequest
[WARNING]   - org.apache.maven.settings.building.SettingsBuildingResult
[WARNING]   - org.apache.maven.settings.io.SettingsParseException
[WARNING]   - org.apache.maven.settings.merge.MavenSettingsMerger
[WARNING]   - org.apache.maven.settings.validation.SettingsValidator
[WARNING]   - org.apache.maven.settings.building.SettingsBuildingException
[WARNING]   - org.apache.maven.settings.crypto.DefaultSettingsDecrypter
[WARNING]   - org.apache.maven.settings.validation.DefaultSettingsValidator
[WARNING]   - org.apache.maven.settings.building.DefaultSettingsProblem
[WARNING]   - 22 more...
[WARNING] aopalliance-1.0.jar, epics-forwarder-1.0.0-bp.jar define 9 overlapping classes: 
[WARNING]   - org.aopalliance.intercept.ConstructorInterceptor
[WARNING]   - org.aopalliance.intercept.MethodInvocation
[WARNING]   - org.aopalliance.intercept.MethodInterceptor
[WARNING]   - org.aopalliance.intercept.Invocation
[WARNING]   - org.aopalliance.aop.AspectException
[WARNING]   - org.aopalliance.intercept.Interceptor
[WARNING]   - org.aopalliance.intercept.Joinpoint
[WARNING]   - org.aopalliance.intercept.ConstructorInvocation
[WARNING]   - org.aopalliance.aop.Advice
[WARNING] epics-forwarder-1.0.0-bp.jar, epics-pvdata-6.1.7-bp.jar define 246 overlapping classes: 
[WARNING]   - org.epics.pvdata.misc.TimeFunctionRequester
[WARNING]   - org.epics.pvdata.factory.BasePVStructureArray
[WARNING]   - org.epics.pvdata.misc.ThreadPriority
[WARNING]   - org.epics.pvdata.pv.PVStringArray
[WARNING]   - org.epics.pvdata.pv.ScalarType$1
[WARNING]   - org.epics.pvdata.misc.LinkedListCreate$LinkedListArrayImpl
[WARNING]   - org.epics.pvdata.factory.BasePVBooleanArray
[WARNING]   - org.epics.pvdata.pv.PVDoubleArray
[WARNING]   - org.epics.pvdata.property.MultiChoiceFactory$MultiChoiceImpl
[WARNING]   - org.epics.pvdata.factory.StandardPVFieldFactory$StandardPVFieldImpl
[WARNING]   - 236 more...
[WARNING] xz-1.8.jar, epics-forwarder-1.0.0-bp.jar define 116 overlapping classes: 
[WARNING]   - org.tukaani.xz.lzma.LZMADecoder$LengthDecoder
[WARNING]   - org.tukaani.xz.index.IndexDecoder
[WARNING]   - org.tukaani.xz.lzma.LZMADecoder
[WARNING]   - org.tukaani.xz.lzma.LZMAEncoderFast
[WARNING]   - org.tukaani.xz.lzma.LZMAEncoder$LengthEncoder
[WARNING]   - org.tukaani.xz.BlockOutputStream
[WARNING]   - org.tukaani.xz.simple.SimpleFilter
[WARNING]   - org.tukaani.xz.BasicArrayCache
[WARNING]   - org.tukaani.xz.rangecoder.RangeCoder
[WARNING]   - org.tukaani.xz.XZOutputStream
[WARNING]   - 106 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-component-annotations-1.6.jar define 3 overlapping classes: 
[WARNING]   - org.codehaus.plexus.component.annotations.Requirement
[WARNING]   - org.codehaus.plexus.component.annotations.Component
[WARNING]   - org.codehaus.plexus.component.annotations.Configuration
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-repository-metadata-3.3.9.jar define 9 overlapping classes: 
[WARNING]   - org.apache.maven.artifact.repository.metadata.Versioning
[WARNING]   - org.apache.maven.artifact.repository.metadata.io.xpp3.MetadataXpp3Reader$1
[WARNING]   - org.apache.maven.artifact.repository.metadata.Metadata
[WARNING]   - org.apache.maven.artifact.repository.metadata.io.xpp3.MetadataXpp3Reader$ContentTransformer
[WARNING]   - org.apache.maven.artifact.repository.metadata.io.xpp3.MetadataXpp3Writer
[WARNING]   - org.apache.maven.artifact.repository.metadata.Plugin
[WARNING]   - org.apache.maven.artifact.repository.metadata.io.xpp3.MetadataXpp3Reader
[WARNING]   - org.apache.maven.artifact.repository.metadata.Snapshot
[WARNING]   - org.apache.maven.artifact.repository.metadata.SnapshotVersion
[WARNING] biz.aQute.bndlib-5.1.1.jar, epics-forwarder-1.0.0-bp.jar, org.osgi.core-6.0.0.jar define 69 overlapping classes: 
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
[WARNING] commons-collections-3.2.jar, epics-forwarder-1.0.0-bp.jar define 458 overlapping classes: 
[WARNING]   - org.apache.commons.collections.map.AbstractLinkedMap$LinkMapIterator
[WARNING]   - org.apache.commons.collections.bidimap.TreeBidiMap$Node
[WARNING]   - org.apache.commons.collections.comparators.TransformingComparator
[WARNING]   - org.apache.commons.collections.iterators.AbstractListIteratorDecorator
[WARNING]   - org.apache.commons.collections.bag.AbstractMapBag$BagIterator
[WARNING]   - org.apache.commons.collections.map.AbstractReferenceMap$ReferenceKeySetIterator
[WARNING]   - org.apache.commons.collections.StaticBucketMap
[WARNING]   - org.apache.commons.collections.ReferenceMap$1
[WARNING]   - org.apache.commons.collections.comparators.ComparatorChain
[WARNING]   - org.apache.commons.collections.KeyValue
[WARNING]   - 448 more...
[WARNING] plexus-utils-3.3.0.jar, epics-forwarder-1.0.0-bp.jar define 105 overlapping classes: 
[WARNING]   - org.codehaus.plexus.util.xml.pull.EntityReplacementMap
[WARNING]   - org.codehaus.plexus.util.introspection.ReflectionValueExtractor
[WARNING]   - org.codehaus.plexus.util.InterpolationFilterReader
[WARNING]   - org.codehaus.plexus.util.cli.shell.CmdShell
[WARNING]   - org.codehaus.plexus.util.cli.shell.Shell
[WARNING]   - org.codehaus.plexus.util.Os
[WARNING]   - org.codehaus.plexus.util.xml.pull.XmlSerializer
[WARNING]   - org.codehaus.plexus.util.cli.CommandLineUtils$2
[WARNING]   - org.codehaus.plexus.util.FastMap
[WARNING]   - org.codehaus.plexus.util.xml.SerializerXMLWriter
[WARNING]   - 95 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, javax.inject-1.jar define 6 overlapping classes: 
[WARNING]   - javax.inject.Inject
[WARNING]   - javax.inject.Singleton
[WARNING]   - javax.inject.Scope
[WARNING]   - javax.inject.Named
[WARNING]   - javax.inject.Provider
[WARNING]   - javax.inject.Qualifier
[WARNING] commons-compress-1.19.jar, epics-forwarder-1.0.0-bp.jar define 354 overlapping classes: 
[WARNING]   - org.apache.commons.compress.archivers.dump.ShortFileException
[WARNING]   - org.apache.commons.compress.compressors.bzip2.BZip2CompressorOutputStream
[WARNING]   - org.apache.commons.compress.archivers.dump.DumpArchiveEntry
[WARNING]   - org.apache.commons.compress.archivers.examples.Archiver$6
[WARNING]   - org.apache.commons.compress.compressors.lz4.XXHash32
[WARNING]   - org.apache.commons.compress.utils.ByteUtils$ByteConsumer
[WARNING]   - org.apache.commons.compress.compressors.lz4.BlockLZ4CompressorOutputStream$1
[WARNING]   - org.apache.commons.compress.compressors.deflate64.HuffmanState
[WARNING]   - org.apache.commons.compress.archivers.zip.ZipArchiveOutputStream$CurrentEntry
[WARNING]   - org.apache.commons.compress.compressors.pack200.Pack200Strategy$2
[WARNING]   - 344 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, commons-lang3-3.4.jar define 224 overlapping classes: 
[WARNING]   - org.apache.commons.lang3.time.StopWatch$State$1
[WARNING]   - org.apache.commons.lang3.builder.MultilineRecursiveToStringStyle
[WARNING]   - org.apache.commons.lang3.SerializationUtils
[WARNING]   - org.apache.commons.lang3.time.FastDatePrinter$TwentyFourHourField
[WARNING]   - org.apache.commons.lang3.LocaleUtils
[WARNING]   - org.apache.commons.lang3.concurrent.ConcurrentException
[WARNING]   - org.apache.commons.lang3.CharRange
[WARNING]   - org.apache.commons.lang3.builder.IDKey
[WARNING]   - org.apache.commons.lang3.reflect.TypeUtils
[WARNING]   - org.apache.commons.lang3.ArrayUtils
[WARNING]   - 214 more...
[WARNING] commons-lang-2.1.jar, epics-forwarder-1.0.0-bp.jar define 110 overlapping classes: 
[WARNING]   - org.apache.commons.lang.NotImplementedException
[WARNING]   - org.apache.commons.lang.math.JVMRandom
[WARNING]   - org.apache.commons.lang.builder.ToStringBuilder
[WARNING]   - org.apache.commons.lang.time.FastDateFormat$TimeZoneDisplayKey
[WARNING]   - org.apache.commons.lang.exception.NestableRuntimeException
[WARNING]   - org.apache.commons.lang.time.FastDateFormat$CharacterLiteral
[WARNING]   - org.apache.commons.lang.time.FastDateFormat$TimeZoneNameRule
[WARNING]   - org.apache.commons.lang.builder.ReflectionToStringBuilder$1
[WARNING]   - org.apache.commons.lang.mutable.MutableDouble
[WARNING]   - org.apache.commons.lang.UnhandledException
[WARNING]   - 100 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, org.eclipse.sisu.plexus-0.3.2.jar, plexus-container-default-1.0-alpha-30.jar define 82 overlapping classes: 
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
[WARNING] aether-api-1.0.2.v20150114.jar, epics-forwarder-1.0.0-bp.jar define 128 overlapping classes: 
[WARNING]   - org.eclipse.aether.SessionData
[WARNING]   - org.eclipse.aether.repository.ProxySelector
[WARNING]   - org.eclipse.aether.transfer.TransferResource
[WARNING]   - org.eclipse.aether.transfer.TransferListener
[WARNING]   - org.eclipse.aether.metadata.MergeableMetadata
[WARNING]   - org.eclipse.aether.repository.LocalMetadataRequest
[WARNING]   - org.eclipse.aether.resolution.DependencyResult
[WARNING]   - org.eclipse.aether.DefaultRepositorySystemSession$NullMirrorSelector
[WARNING]   - org.eclipse.aether.transfer.NoRepositoryConnectorException
[WARNING]   - org.eclipse.aether.metadata.Metadata$Nature
[WARNING]   - 118 more...
[WARNING] maven-dependency-tree-3.0.jar, epics-forwarder-1.0.0-bp.jar define 20 overlapping classes: 
[WARNING]   - org.apache.maven.shared.dependency.graph.traversal.BuildingDependencyNodeVisitor
[WARNING]   - org.apache.maven.shared.dependency.graph.DependencyGraphBuilderException
[WARNING]   - org.apache.maven.shared.dependency.graph.traversal.DependencyNodeVisitor
[WARNING]   - org.apache.maven.shared.dependency.graph.traversal.CollectingDependencyNodeVisitor
[WARNING]   - org.apache.maven.shared.dependency.graph.internal.DefaultDependencyGraphBuilder
[WARNING]   - org.apache.maven.shared.dependency.graph.traversal.SerializingDependencyNodeVisitor$GraphTokens
[WARNING]   - org.apache.maven.shared.dependency.graph.traversal.FilteringDependencyNodeVisitor
[WARNING]   - org.apache.maven.shared.dependency.graph.internal.Invoker
[WARNING]   - org.apache.maven.shared.dependency.graph.internal.Maven3DependencyGraphBuilder
[WARNING]   - org.apache.maven.shared.dependency.graph.internal.Maven31DependencyGraphBuilder
[WARNING]   - 10 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, org.osgi.compendium-4.2.0.jar define 177 overlapping classes: 
[WARNING]   - org.osgi.util.measurement.State
[WARNING]   - org.osgi.service.blueprint.container.EventConstants
[WARNING]   - org.osgi.service.blueprint.reflect.ReferenceMetadata
[WARNING]   - org.osgi.service.blueprint.container.ComponentDefinitionException
[WARNING]   - org.osgi.service.io.ConnectionFactory
[WARNING]   - info.dmtree.security.DmtPrincipalPermissionCollection
[WARNING]   - info.dmtree.security.AlertPermission
[WARNING]   - org.osgi.service.blueprint.reflect.ServiceMetadata
[WARNING]   - info.dmtree.Uri$1
[WARNING]   - org.osgi.service.useradmin.UserAdminPermission
[WARNING]   - 167 more...
[WARNING] doxia-decoration-model-1.0.jar, epics-forwarder-1.0.0-bp.jar define 18 overlapping classes: 
[WARNING]   - org.apache.maven.doxia.site.decoration.Banner
[WARNING]   - org.apache.maven.doxia.site.decoration.inheritance.PathUtils
[WARNING]   - org.apache.maven.doxia.site.decoration.io.xpp3.DecorationXpp3Reader
[WARNING]   - org.apache.maven.doxia.site.decoration.inheritance.DecorationModelInheritanceAssembler
[WARNING]   - org.apache.maven.doxia.site.decoration.Body
[WARNING]   - org.apache.maven.doxia.site.decoration.DecorationModel
[WARNING]   - org.apache.maven.doxia.site.decoration.inheritance.DefaultDecorationModelInheritanceAssembler
[WARNING]   - org.apache.maven.doxia.site.decoration.io.xpp3.DecorationXpp3Writer
[WARNING]   - org.apache.maven.doxia.site.decoration.MenuItem
[WARNING]   - org.apache.maven.doxia.site.decoration.inheritance.DefaultDecorationModelInheritanceAssembler$URLContainer
[WARNING]   - 8 more...
[WARNING] biz.aQute.bndlib-5.1.1.jar, epics-forwarder-1.0.0-bp.jar define 1152 overlapping classes: 
[WARNING]   - aQute.lib.exceptions.RunnableWithException
[WARNING]   - aQute.bnd.component.annotations.ServiceScope
[WARNING]   - aQute.lib.io.CharBufferReader
[WARNING]   - aQute.lib.collections.DoubleKeyMap$1
[WARNING]   - aQute.bnd.connection.settings.SettingsDTO
[WARNING]   - org.osgi.service.repository.IdentityExpression
[WARNING]   - aQute.bnd.build.model.conversions.HeaderClauseFormatter
[WARNING]   - aQute.bnd.classfile.ModuleAttribute$Open
[WARNING]   - aQute.bnd.build.Project
[WARNING]   - aQute.bnd.url.HttpsVerification
[WARNING]   - 1142 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-reporting-api-3.0.jar define 4 overlapping classes: 
[WARNING]   - org.apache.maven.reporting.MavenReport$1
[WARNING]   - org.apache.maven.reporting.MavenMultiPageReport
[WARNING]   - org.apache.maven.reporting.MavenReportRenderer
[WARNING]   - org.apache.maven.reporting.MavenReport
[WARNING] maven-core-3.3.9.jar, epics-forwarder-1.0.0-bp.jar define 394 overlapping classes: 
[WARNING]   - org.apache.maven.artifact.metadata.ResolutionGroup
[WARNING]   - org.apache.maven.artifact.handler.DefaultArtifactHandler
[WARNING]   - org.apache.maven.plugin.prefix.NoPluginFoundForPrefixException
[WARNING]   - org.apache.maven.lifecycle.internal.DependencyContext
[WARNING]   - org.apache.maven.project.path.PathTranslator
[WARNING]   - org.apache.maven.plugin.PluginDescriptorCache
[WARNING]   - org.apache.maven.lifecycle.internal.LifecycleDependencyResolver
[WARNING]   - org.apache.maven.plugin.PluginConfigurationException
[WARNING]   - org.apache.maven.plugin.CompoundMojoExecutionListener
[WARNING]   - org.apache.maven.artifact.repository.metadata.RepositoryMetadata
[WARNING]   - 384 more...
[WARNING] biz.aQute.bndlib-5.1.1.jar, epics-forwarder-1.0.0-bp.jar, org.osgi.core-6.0.0.jar, org.osgi.compendium-4.2.0.jar define 8 overlapping classes: 
[WARNING]   - org.osgi.util.tracker.ServiceTrackerCustomizer
[WARNING]   - org.osgi.util.tracker.ServiceTracker
[WARNING]   - org.osgi.util.tracker.BundleTracker$Tracked
[WARNING]   - org.osgi.util.tracker.BundleTracker
[WARNING]   - org.osgi.util.tracker.BundleTrackerCustomizer
[WARNING]   - org.osgi.util.tracker.ServiceTracker$AllTracked
[WARNING]   - org.osgi.util.tracker.AbstractTracked
[WARNING]   - org.osgi.util.tracker.ServiceTracker$Tracked
[WARNING] velocity-1.5.jar, epics-forwarder-1.0.0-bp.jar define 246 overlapping classes: 
[WARNING]   - org.apache.velocity.runtime.parser.node.ASTFloatingPointLiteral
[WARNING]   - org.apache.velocity.app.event.EventHandlerMethodExecutor
[WARNING]   - org.apache.velocity.io.UnicodeInputStream
[WARNING]   - org.apache.velocity.exception.TemplateInitException
[WARNING]   - org.apache.velocity.runtime.parser.node.PutExecutor
[WARNING]   - org.apache.velocity.runtime.parser.node.ASTStop
[WARNING]   - org.apache.velocity.util.introspection.Info
[WARNING]   - org.apache.velocity.util.introspection.UberspectImpl$VelMethodImpl
[WARNING]   - org.apache.velocity.runtime.resource.loader.DataSourceResourceLoader
[WARNING]   - org.apache.velocity.app.event.implement.InvalidReferenceInfo
[WARNING]   - 236 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, plexus-container-default-1.0-alpha-30.jar define 130 overlapping classes: 
[WARNING]   - org.codehaus.plexus.configuration.PlexusConfigurationMerger
[WARNING]   - org.codehaus.plexus.lifecycle.LifecycleHandler
[WARNING]   - org.codehaus.plexus.personality.plexus.lifecycle.phase.ServiceablePhase
[WARNING]   - org.codehaus.plexus.component.manager.DefaultComponentManagerManager
[WARNING]   - org.codehaus.plexus.component.repository.ComponentProfile
[WARNING]   - org.codehaus.plexus.lifecycle.BasicLifecycleHandler
[WARNING]   - org.codehaus.plexus.component.discovery.AbstractComponentDiscoverer
[WARNING]   - org.codehaus.plexus.personality.plexus.lifecycle.phase.AutoConfigurePhase
[WARNING]   - org.codehaus.plexus.personality.plexus.lifecycle.phase.ContextualizePhase
[WARNING]   - org.codehaus.plexus.component.composition.CompositionResolver
[WARNING]   - 120 more...
[WARNING] epics-forwarder-1.0.0-bp.jar, maven-compat-3.3.9.jar, maven-reporting-api-3.0.jar define 1 overlapping classes: 
[WARNING]   - org.apache.maven.reporting.MavenReportException
[WARNING] maven-shared-utils-3.2.1.jar, epics-forwarder-1.0.0-bp.jar define 84 overlapping classes: 
[WARNING]   - org.apache.maven.shared.utils.io.FileUtils$FilterWrapper
[WARNING]   - org.apache.maven.shared.utils.logging.LoggerLevelRenderer
[WARNING]   - org.apache.maven.shared.utils.io.DirectoryScanner
[WARNING]   - org.apache.maven.shared.utils.cli.javatool.AbstractJavaTool$3
[WARNING]   - org.apache.maven.shared.utils.StringUtils
[WARNING]   - org.apache.maven.shared.utils.cli.StreamConsumer
[WARNING]   - org.apache.maven.shared.utils.cli.shell.Shell
[WARNING]   - org.apache.maven.shared.utils.introspection.ReflectionValueExtractor
[WARNING]   - org.apache.maven.shared.utils.introspection.ClassMap$1
[WARNING]   - org.apache.maven.shared.utils.Expand
[WARNING]   - 74 more...
[WARNING] org.eclipse.sisu.inject-0.3.2.jar, epics-forwarder-1.0.0-bp.jar define 270 overlapping classes: 
[WARNING]   - org.eclipse.sisu.wire.BeanProviders$3
[WARNING]   - org.eclipse.sisu.space.WildcardKey$Qualified
[WARNING]   - org.eclipse.sisu.inject.RankedSequence
[WARNING]   - org.eclipse.sisu.space.asm.Frame
[WARNING]   - org.eclipse.sisu.inject.MildValues$Soft
[WARNING]   - org.eclipse.sisu.wire.ParameterKeys
[WARNING]   - org.eclipse.sisu.inject.MildValues$EntryItr
[WARNING]   - org.eclipse.sisu.space.NamedClass
[WARNING]   - org.eclipse.sisu.inject.DeferredProvider
[WARNING]   - org.eclipse.sisu.wire.EntryListAdapter$ValueListIterator
[WARNING]   - 260 more...
[WARNING] biz.aQute.bndlib-5.1.1.jar, epics-forwarder-1.0.0-bp.jar, org.osgi.compendium-4.2.0.jar define 4 overlapping classes: 
[WARNING]   - org.osgi.service.log.LogListener
[WARNING]   - org.osgi.service.log.LogReaderService
[WARNING]   - org.osgi.service.log.LogEntry
[WARNING]   - org.osgi.service.log.LogService
[WARNING] maven-shade-plugin has detected that some class files are
[WARNING] present in two or more JARs. When this happens, only one
[WARNING] single version of the class is copied to the uber jar.
[WARNING] Usually this is not harmful and you can skip these warnings,
[WARNING] otherwise try to manually exclude artifacts based on
[WARNING] mvn dependency:tree -Ddetail=true and the above output.
[WARNING] See http://maven.apache.org/plugins/maven-shade-plugin/
[INFO] Replacing original artifact with shaded artifact.
[INFO] Replacing epics-forwarder/target/epics-forwarder-1.0.0-bp.jar with epics-forwarder/target/epics-forwarder-1.0.0-bp-shaded.jar
[INFO] Dependency-reduced POM written at: epics-forwarder/dependency-reduced-pom.xml
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ epics-forwarder ---
[INFO] Installing epics-forwarder/target/epics-forwarder-1.0.0-bp.jar to ~/.m2/repository/org/epics/epics-forwarder/1.0.0-bp/epics-forwarder-1.0.0-bp.jar
[INFO] Installing epics-forwarder/dependency-reduced-pom.xml to ~/.m2/repository/org/epics/epics-forwarder/1.0.0-bp/epics-forwarder-1.0.0-bp.pom
[INFO] ------------------------------------------------------------------------
[INFO] Reactor Summary:
[INFO] 
[INFO] EPICS Core Server BackPort 1.5 Build 7.0.8-bp  SUCCESS [  0.347 s]
[INFO] org.epics.util 1.0.5-bp ...................... SUCCESS [  2.776 s]
[INFO] pvDataJava 6.1.7-bp .......................... SUCCESS [  0.194 s]
[INFO] pvAccessJava 5.1.7-bp ........................ SUCCESS [  0.636 s]
[INFO] normativeTypesJava 0.3.7-bp .................. SUCCESS [  0.073 s]
[INFO] epics-forwarder 1.0.0-bp ..................... SUCCESS [  2.590 s]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  6.769 s
[INFO] Finished at: 2021-10-31T00:24:08+02:00
[INFO] ------------------------------------------------------------------------

Process finished with exit code 0


```

3. Copy jar to VMS
pvDataJava, pvAccessJava, normativeTypesJava  and org.epics.util don't need to be copiied to VMS.  They only need to be in your ~/m2 directory for when you build AIDA-PVA.JAR.  

The only Jar to copy to VMS is the Forwarder.
```shell
cp ~/.m2/repository/org/epics/epics-forwarder/1.0.0-bp/epics-forwarder-1.0.0-bp.jar epics-forwarder.jar
sftp mccdev
Connecting to mccdev...
 
sftp> mput epics-forwarder.jar

```

4. Move code to SLCIMAGE
On VMS
```shell
MCCDEV> COPY EPICS_FORWARDER.JAR SLCIMAGE:EPICS_FORWARDER.JAR
```
