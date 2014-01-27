--- src/sdcv.cpp.orig
+++ src/sdcv.cpp
@@ -161,7 +161,11 @@
 
 	strlist_t dicts_dir_list;
 
-	dicts_dir_list.push_back(std::string(g_get_home_dir())+G_DIR_SEPARATOR+
+	const char *homedir = g_getenv ("HOME");
+	if (!homedir)
+		homedir = g_get_home_dir ();
+
+	dicts_dir_list.push_back(std::string(homedir)+G_DIR_SEPARATOR+
 				 ".stardict"+G_DIR_SEPARATOR+"dic");
 	dicts_dir_list.push_back(data_dir);   
 
