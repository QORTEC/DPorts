
$FreeBSD: sysutils/installwatch/files/patch-installwatch.c 340725 2014-01-22 17:40:44Z mat $

--- installwatch.c.orig
+++ installwatch.c
@@ -574,7 +574,7 @@
 
 	REFCOUNT;
 	va_start(ap, flags);
-	mode = va_arg(ap, mode_t);
+	mode = va_arg(ap, int);
 	va_end(ap);
 	canonicalize(pathname, canonic);
 
