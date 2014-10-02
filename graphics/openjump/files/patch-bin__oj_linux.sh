--- bin/oj_linux.sh.orig	2014-03-16 17:15:30.000000000 +0100
+++ bin/oj_linux.sh	2014-09-07 13:27:15.000000000 +0200
@@ -4,9 +4,10 @@
 ## if unset defaults to
 ##   JUMP_HOME (oj app folder) if writable or $HOME/.openjump (user home)
 #JUMP_SETTINGS="/tmp/foobar"
+JUMP_SETTINGS="$HOME/.openjump"
 
 ## uncomment and put the path to your jre here
-#JAVA_HOME="/home/ed/jre1.6.0_21"
+JAVA_HOME=${JAVA_HOME}
 
 ## uncomment and change your memory configuration here 
 ## Xms is initial size, Xmx is maximum size
@@ -43,6 +44,7 @@
   # extract zipped files in native dir (our way to ship symlinks to desktops)
   for filepath in $(find "$1/" -name '*.tgz' -o -name '*.tar.gz')
   do
+    echo "#####  filepath = '$filepath'"
     file=$(basename "$filepath")
     folder=$(dirname "$filepath")
     done=".$file.unzipped"
@@ -66,6 +68,7 @@
 
 macinstall(){
   # create app package
+  echo "#####  MACINSTALL"
   cp -R -a "$1"/bin/OpenJUMP.app/Contents "$1" &&\
   awk '{sub(/..\/oj_/,"bin/oj_",$0)}1' "$1"/bin/OpenJUMP.app/Contents/Resources/script > "$1"/Contents/Resources/script &&\
   echo patched oj.app
@@ -74,12 +77,14 @@
 }
 
 ## detect home folder
+echo "#####  \$0 = '$0'"
 if(test -L "$0") then
   auxlink=`ls -l "$0" | sed 's/^[^>]*-> //g'`
   JUMP_HOME=`dirname "$auxlink"`/..
 else 
   JUMP_HOME=`dirname "$0"`/..
 fi
+echo "#####  JUMP_HOME = '$JUMP_HOME'"
 
 ## run postinstalls only, if requested
 case "$1" in
@@ -95,13 +100,20 @@
 
 ## cd into jump home
 OLD_DIR=`pwd`
+echo "#####  pwd = '$OLD_DIR'"
 cd "$JUMP_HOME"
+PWD_DIR=`pwd`
+echo "#####  cd '$JUMP_HOME', pwd = '$PWD_DIR'"
+
 
 ## determine where to place settings, if no path given
+echo "#####  ===== JUMP_SETTINGS = '$JUMP_SETTINGS'"
 [ -z "$JUMP_SETTINGS" ] && \
 JUMP_SETTINGS="$JUMP_HOME"; \
 if [ -d "$JUMP_SETTINGS" ]; then
+  echo "#####  within -d \$JUMP_SETTINGS"
   if [ ! -w "$JUMP_SETTINGS" ]; then
+    echo "#####  within ! -w \$JUMP_SETTINGS"
     # try users home dir
     JUMP_SETTINGS="$HOME/.openjump"
     # create if missing
@@ -118,14 +130,19 @@
 # 1. first in oj_home/jre
 # 2. in configured java_home
 # 3. in path
+echo "#####  -----------------------------------------------------------"
+echo "#####  SEARCH JAVA:"
 if [ -f "$JUMP_HOME/jre/bin/java" ]; then
   JAVA="$JUMP_HOME/jre/bin/java"
+  echo "#####  JAVA = '$JAVA'"
 # is there a jre defined by env var?
 elif [ -n "$JAVA_HOME" ]; then
   JAVA=$JAVA_HOME/bin/java
+  echo "#####  JAVA = '$JAVA'"
 # well, let's look what we've got in the path
 else
   JAVA=`which java`
+  echo "#####  JAVA = '$JAVA'"
 fi
 
 # java available
@@ -134,20 +151,26 @@
 add the location of java to your PATH environment variable." && ERROR=1 && end
 
 # resolve recursive links to java binary
+echo "#####  ===== Resolve recursive links to java binary: \$1 = '$1'"
 relPath(){ echo $1 | awk '/^\//{exit 1}'; }
+echo "#####  awk script survived after \$1 test"
 relPath "$JAVA" && JAVA="$(pwd)/$JAVA"
-while [ -L "${JAVA}" ]; do
-  JDIR=$(dirname "$JAVA")
-  JAVA=$(readlink -n "${JAVA}")
-  relPath "$JAVA" && JAVA="${JDIR}/${JAVA}"
-done
+#while [ -L "${JAVA}" ]; do
+#  JDIR=$(dirname "$JAVA")
+#  echo "#####    JDIR = '$JDIR'"
+#  JAVA=$(readlink -n "${JAVA}")
+#  echo "#####    JAVA = '$JAVA'"
+#  relPath "$JAVA" && JAVA="${JDIR}/${JAVA}"
+#done
 # java executable file?
 [ ! -x "$JAVA" ] && \
  echo "The found java binary '$JAVA' is no executable file." && ERROR=1 && end
 
 # java version check
 JAVA_VERSIONSTRING="$("$JAVA" -version 2>&1)"
-JAVA_VERSION=$(echo $JAVA_VERSIONSTRING | awk -F'"' '/^java version/{print $2}' | awk -F'.' '{print $1"."$2}')
+echo "#####  JAVA_VERSIONSTRING = '$JAVA_VERSIONSTRING'"
+JAVA_VERSION=$(echo $JAVA_VERSIONSTRING | awk -F'"' '/^java version/ || /^openjdk version/{print $2}' | awk -F'.' '{print $1"."$2}')
+echo "#####  JAVA_VERSION = '$JAVA_VERSION'"
 JAVA_ARCH=$(echo $JAVA_VERSIONSTRING | grep -q -i 64-bit && echo x64 || echo x86)
 JAVA_NEEDED="1.5"
 if ! awk "BEGIN{if($JAVA_VERSION < $JAVA_NEEDED)exit 1}"; then
@@ -157,7 +180,11 @@
 fi
 
 # use previously set or detect RAM size in bytes
-RAM_SIZE=${RAM_SIZE-$(expr "$(awk '/MemTotal/{print $2}' /proc/meminfo)" \* 1024)}
+#RAM_SIZE=${RAM_SIZE-$(expr "$(awk '/MemTotal/{print $2}' /proc/meminfo)" \* 1024)}
+RAM_SIZE=${RAM_SIZE-$(expr "$(sysctl hw.realmem | awk '/hw.realmem:/{print $2}')" / 1024)}
+echo "#####  -----------------------------------------------------------"
+echo "#####  RAM_SIZE = '$RAM_SIZE'"
+
 if [ -n "$JAVA_MAXMEM" ]; then
   echo "max. memory limit defined via JAVA_MAXMEM=$JAVA_MAXMEM"
 elif ! is_number "$RAM_SIZE"; then
@@ -175,14 +202,18 @@
   else
     MEM_MAX="$MEM_MINUS1GB"
   fi
+  echo "#####  MEM_MAX = '$MEM_MAX'"
 
   # limit 32bit jre to 3GiB = 3221225472 bytes
+  echo "#####  JAVA_ARCH = '$JAVA_ARCH'"
   if [ "$JAVA_ARCH" != "x64" ] && [ "$MEM_MAX" -gt "3221225472" ]; then
     MEM_MAX=3221225472
   fi
 
-  MEM_MAX_MB=`expr $MEM_MAX / 1024 / 1024`
+  MEM_MAX_MB=`expr $MEM_MAX / 1024`
   JAVA_MAXMEM="-Xmx${MEM_MAX_MB}M"
+  echo "#####  JAVA_MAXMEM = '$JAVA_MAXMEM'"
+
   # output info
   echo limit max. memory to $MEM_MAX_MB MiB
 fi
@@ -190,7 +221,7 @@
 # always print java infos
 echo "Running -> '${JAVA}'; " $("$JAVA" -version 2>&1|awk 'BEGIN{ORS=""}{print $0"; "}')
 
-JUMP_PROFILE=~/.jump/openjump.profile
+JUMP_PROFILE="$HOME/.openjump/openjump.profile"
 if [ -f "$JUMP_PROFILE" ]; then
   source $JUMP_PROFILE
 fi
@@ -199,8 +230,13 @@
 if [ -z "$JUMP_LIB" ]; then
   JUMP_LIB="./lib"
 fi
+echo "#####  JUMP_LIB = '$JUMP_LIB'"
+
 JUMP_NATIVE_DIR="$JUMP_LIB/native"
 JUMP_PLUGIN_DIR="${JUMP_PLUGIN_DIR:=$JUMP_LIB/ext}"
+echo "#####  -----------------------------------------------------------"
+echo "#####  JUMP_NATIVE_DIR = '$JUMP_NATIVE_DIR'"
+echo "#####  JUMP_PLUGIN_DIR = '$JUMP_PLUGIN_DIR'"
 
 JUMP_PLUGINS=./bin/default-plugins.xml
 if [ -z "$JUMP_PLUGINS" ] || [ ! -f "$JUMP_PLUGINS" ]; then
@@ -209,6 +245,7 @@
     JUMP_PLUGINS="./scripts/default-plugins.xml"
   fi
 fi
+echo "#####  JUMP_PLUGINS = '$JUMP_PLUGINS'"
 
 # include every jar/zip in lib and native dir
 for libfile in "$JUMP_LIB/"*.zip "$JUMP_LIB/"*.jar "$JUMP_NATIVE_DIR/"*.jar
@@ -217,29 +254,39 @@
 done
 CLASSPATH=.:./bin:./conf:$CLASSPATH
 export CLASSPATH;
+echo "#####  CLASSPATH = '$CLASSPATH'"
 
 ## compile jump opts
 #
 JUMP_OPTS="-plug-in-directory $JUMP_PLUGIN_DIR"
+echo "#####  JUMP_OPTS = '$JUMP_OPTS'"
 if [ -f "$JUMP_PLUGINS" ]; then
   JUMP_OPTS="$JUMP_OPTS -default-plugins $JUMP_PLUGINS"
+  echo "#####  JUMP_OPTS = '$JUMP_OPTS'"
 fi
+
 # workbench-properties.xml is used to manually load plugins (ISA uses this)
 JUMP_PROPERTIES=./bin/workbench-properties.xml
 if [ -n "$JUMP_PROPERTIES" ] && [ -f "$JUMP_PROPERTIES" ]; then
   JUMP_OPTS="$JUMP_OPTS -properties $JUMP_PROPERTIES"
+  echo "#####  JUMP_OPTS = '$JUMP_OPTS'"
 fi
 
 # compile jre opts, respect already set ones from e.g. mac
 JAVA_OPTS=""
+echo "#####  JAVA_OPTS = '$JAVA_OPTS'"
 JAVA_OPTS="$JAVA_OPTS $JAVA_MAXMEM $JAVA_LANG"
+echo "#####  JAVA_OPTS = '$JAVA_OPTS'"
 JAVA_OPTS="$JAVA_OPTS -Djump.home=."
+echo "#####  JAVA_OPTS = '$JAVA_OPTS'"
 [ -n "JAVA_SAXDRIVER"    ] && JAVA_OPTS="$JAVA_OPTS -Dorg.xml.sax.driver=$JAVA_SAXDRIVER"
 [ -n "$JAVA_LOOKANDFEEL" ] && JAVA_OPTS="$JAVA_OPTS -Dswing.defaultlaf=$JAVA_LOOKANDFEEL"
 JAVA_OPTS="$JAVA_OPTS $JAVA_OPTS_OVERRIDE"
+echo "#####  JAVA_OPTS = '$JAVA_OPTS'"
 
 # in case some additional archives were placed in native dir inbetween
-extract_libs "$JUMP_NATIVE_DIR"
+echo "#####  -----------------------------------------------------------"
+#extract_libs "$JUMP_NATIVE_DIR"
 
 # allow jre to find native libraries in native dir, lib/ext (backwards compatibility)
 # NOTE: mac osx DYLD_LIBRARY_PATH is set in oj_macosx.command only
@@ -265,4 +312,4 @@
 cd "$OLD_DIR"
 
 ## run end function
-end
\ No newline at end of file
+end
