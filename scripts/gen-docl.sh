# To Generate documentation for AIDA-PVA
# Make sure DEV_HOME is set to the root where the aida-pva and aida-pva-tests directories is located
# make sure you've got lombok-1.18.20.jar (You will have if you've ever built the test project with maven)
# This script will
#    - First de-lombok the code
#    - Then run doxygen
#    - Finally update the documentation in SLAC using rsync

START_TIME=$(date +%s)

cd "${DEV_HOME}/aida-pva" || exit 1
rm -rf root
java -jar ~/.m2/repository/org/projectlombok/lombok/1.18.20/lombok-1.18.20.jar delombok "${DEV_HOME}/aida-pva-tests/src/main/java" -d root/test/java 2>/dev/null
java -jar ~/.m2/repository/org/projectlombok/lombok/1.18.20/lombok-1.18.20.jar delombok src -d root 2>/dev/null

doxygen docs/doxygenConfig
echo "Documentation Generated"
DOC_END_TIME=$(date +%s)

rsync -avzcP --delete --delete-excluded docGen/html/* /afs/slac/www/grp/cd/soft/aida/aida-pva
TRANSFER_END_TIME=$(date +%s)
echo "Documentation Site Updated"

echo "Preparation time: $((DOC_END_TIME - START_TIME)) seconds"
echo "Transfer time: $((TRANSFER_END_TIME - DOC_END_TIME)) seconds"
