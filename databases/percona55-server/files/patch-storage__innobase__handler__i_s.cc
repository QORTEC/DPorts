--- storage/innobase/handler/i_s.cc.orig	2013-12-02 01:43:24.000000000 -0500
+++ storage/innobase/handler/i_s.cc	2014-01-19 12:21:24.113243651 -0500
@@ -49,17 +49,21 @@
 #include "buf0buddy.h"
 #include "buf0buf.h"
 #include "buf0lru.h"
+#include "dict0load.h"
+#include "buf0lru.h"
 #include "ibuf0ibuf.h"
 #include "dict0mem.h"
 #include "dict0types.h"
 #include "dict0boot.h"
 #include "dict0load.h"
 #include "ha_prototypes.h"
+#include "srv0srv.h"
 #include "srv0start.h"
 #include "srv0srv.h"
 #include "trx0i_s.h"
 #include "trx0rseg.h"
 #include "trx0trx.h"
+#include "trx0trx.h"
 #include "trx0undo.h"
 #include "log0online.h"
 #include "btr0btr.h"
