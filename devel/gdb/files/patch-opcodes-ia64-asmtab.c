--- opcodes/ia64-asmtab.c.orig	2013-04-28 14:55:03.000000000 +0200
+++ opcodes/ia64-asmtab.c	2013-04-28 14:56:48.000000000 +0200
@@ -102,7 +102,7 @@
   { "CPUID#", 7, 0, 5, -1, NULL, },
   { "CR[CMCV]", 29, 0, 3, 74, NULL, },
   { "CR[DCR]", 29, 0, 3, 0, NULL, },
-  { "CR[EOI]", 29, 0, 7, 67, "SC Section 5.8.3.4, \"End of External Interrupt Register (EOI � CR67)\" on page 2:119", },
+  { "CR[EOI]", 29, 0, 7, 67, "SC Section 5.8.3.4, \"End of External Interrupt Register (EOI CR67)\" on page 2:119", },
   { "CR[GPTA]", 29, 0, 3, 9, NULL, },
   { "CR[IFA]", 29, 0, 1, 20, NULL, },
   { "CR[IFA]", 29, 0, 3, 20, NULL, },
@@ -124,13 +124,13 @@
   { "CR[ITM]", 29, 0, 3, 1, NULL, },
   { "CR[ITV]", 29, 0, 3, 72, NULL, },
   { "CR[IVA]", 29, 0, 4, 2, NULL, },
-  { "CR[IVR]", 29, 0, 7, 65, "SC Section 5.8.3.2, \"External Interrupt Vector Register (IVR � CR65)\" on page 2:118", },
-  { "CR[LID]", 29, 0, 7, 64, "SC Section 5.8.3.1, \"Local ID (LID � CR64)\" on page 2:117", },
+  { "CR[IVR]", 29, 0, 7, 65, "SC Section 5.8.3.2, \"External Interrupt Vector Register (IVR CR65)\" on page 2:118", },
+  { "CR[LID]", 29, 0, 7, 64, "SC Section 5.8.3.1, \"Local ID (LID CR64)\" on page 2:117", },
   { "CR[LRR%], % in 0 - 1", 10, 0, 3, -1, NULL, },
   { "CR[PMV]", 29, 0, 3, 73, NULL, },
   { "CR[PTA]", 29, 0, 3, 8, NULL, },
   { "CR[TPR]", 29, 0, 3, 66, NULL, },
-  { "CR[TPR]", 29, 0, 7, 66, "SC Section 5.8.3.3, \"Task Priority Register (TPR � CR66)\" on page 2:119", },
+  { "CR[TPR]", 29, 0, 7, 66, "SC Section 5.8.3.3, \"Task Priority Register (TPR  CR66)\" on page 2:119", },
   { "CR[TPR]", 29, 0, 1, 66, NULL, },
   { "CR%, % in 3-7, 10-15, 18, 28-63, 75-79, 82-127", 11, 0, 0, -1, NULL, },
   { "DAHR%, % in 0-7", 12, 0, 1, -1, NULL, },
@@ -305,7 +305,7 @@
   { "CPUID#", 7, 1, 0, -1, NULL, },
   { "CR[CMCV]", 29, 1, 2, 74, NULL, },
   { "CR[DCR]", 29, 1, 2, 0, NULL, },
-  { "CR[EOI]", 29, 1, 7, 67, "SC Section 5.8.3.4, \"End of External Interrupt Register (EOI � CR67)\" on page 2:119", },
+  { "CR[EOI]", 29, 1, 7, 67, "SC Section 5.8.3.4, \"End of External Interrupt Register (EOI CR67)\" on page 2:119", },
   { "CR[GPTA]", 29, 1, 2, 9, NULL, },
   { "CR[IFA]", 29, 1, 2, 20, NULL, },
   { "CR[IFS]", 29, 1, 2, 23, NULL, },
