--- src/qemu/qemu_capabilities.c.orig	2022-12-01 09:55:15 UTC
+++ src/qemu/qemu_capabilities.c
@@ -679,6 +679,9 @@ VIR_ENUM_IMPL(virQEMUCaps,
               "query-stats-schemas", /* QEMU_CAPS_QUERY_STATS_SCHEMAS */
               "sgx-epc", /* QEMU_CAPS_SGX_EPC */
               "thread-context", /* QEMU_CAPS_THREAD_CONTEXT */
+
+              /* 436 */
+              "nvmm", /* QEMU_CAPS_NVMM */
     );
 
 
@@ -765,6 +768,7 @@ struct _virQEMUCaps {
     /* Capabilities which may differ depending on the accelerator. */
     virQEMUCapsAccel kvm;
     virQEMUCapsAccel hvf;
+    virQEMUCapsAccel nvmm;
     virQEMUCapsAccel tcg;
 };
 
@@ -881,6 +885,8 @@ virQEMUCapsAccelStr(virDomainVirtType ty
         return "kvm";
     else if (type == VIR_DOMAIN_VIRT_HVF)
         return "hvf";
+    else if (type == VIR_DOMAIN_VIRT_NVMM)
+        return "nvmm";
 
     return "tcg";
 }
@@ -894,6 +900,8 @@ virQEMUCapsGetAccel(virQEMUCaps *qemuCap
         return &qemuCaps->kvm;
     else if (type == VIR_DOMAIN_VIRT_HVF)
         return &qemuCaps->hvf;
+    else if (type == VIR_DOMAIN_VIRT_NVMM)
+        return &qemuCaps->nvmm;
 
     return &qemuCaps->tcg;
 }
@@ -1014,6 +1022,8 @@ virQEMUCapsGetMachineTypesCaps(virQEMUCa
         accel = &qemuCaps->kvm;
     else if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_HVF))
         accel = &qemuCaps->hvf;
+    else if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM))
+        accel = &qemuCaps->nvmm;
     else
         accel = &qemuCaps->tcg;
 
@@ -1129,6 +1139,11 @@ virQEMUCapsInitGuestFromBinary(virCaps *
                                       NULL, NULL, 0, NULL);
     }
 
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM)) {
+        virCapabilitiesAddGuestDomain(guest, VIR_DOMAIN_VIRT_NVMM,
+                                      NULL, NULL, 0,  NULL);
+    }
+
     if ((ARCH_IS_X86(guestarch) || guestarch == VIR_ARCH_AARCH64))
         virCapabilitiesAddGuestFeatureWithToggle(guest, VIR_CAPS_GUEST_FEATURE_TYPE_ACPI,
                                                  true, true);
@@ -1995,6 +2010,7 @@ virQEMUCaps *virQEMUCapsNewCopy(virQEMUC
 
     if (virQEMUCapsAccelCopy(&ret->kvm, &qemuCaps->kvm) < 0 ||
         virQEMUCapsAccelCopy(&ret->hvf, &qemuCaps->hvf) < 0 ||
+	virQEMUCapsAccelCopy(&ret->nvmm, &qemuCaps->nvmm) < 0 ||
         virQEMUCapsAccelCopy(&ret->tcg, &qemuCaps->tcg) < 0)
         return NULL;
 
@@ -2056,6 +2072,7 @@ void virQEMUCapsDispose(void *obj)
 
     virQEMUCapsAccelClear(&qemuCaps->kvm);
     virQEMUCapsAccelClear(&qemuCaps->hvf);
+    virQEMUCapsAccelClear(&qemuCaps->nvmm);
     virQEMUCapsAccelClear(&qemuCaps->tcg);
 }
 
@@ -2317,6 +2334,10 @@ virQEMUCapsIsVirtTypeSupported(virQEMUCa
         virQEMUCapsGet(qemuCaps, QEMU_CAPS_HVF))
         return true;
 
+    if (virtType == VIR_DOMAIN_VIRT_NVMM &&
+        virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM))
+        return true;
+
     if (virtType == VIR_DOMAIN_VIRT_KVM &&
         virQEMUCapsGet(qemuCaps, QEMU_CAPS_KVM))
         return true;
@@ -3269,6 +3290,22 @@ virQEMUCapsProbeHVF(virQEMUCaps *qemuCap
 }
 #endif
 
+static int
+virQEMUCapsProbeQMPNVMMState(virQEMUCaps *qemuCaps,
+                            qemuMonitor *mon)
+{
+    bool enabled = false;
+    bool present = false;
+
+    if (qemuMonitorGetNVMMState(mon, &enabled, &present) < 0)
+        return -1;
+
+    if (present && enabled)
+        virQEMUCapsSet(qemuCaps, QEMU_CAPS_NVMM);
+
+    return 0;
+}
+
 struct virQEMUCapsCommandLineProps {
     const char *option;
     const char *param;
@@ -4612,6 +4649,10 @@ virQEMUCapsLoadCache(virArch hostArch,
         virQEMUCapsLoadAccel(qemuCaps, ctxt, VIR_DOMAIN_VIRT_HVF) < 0) {
         return -1;
     }
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM) &&
+        virQEMUCapsLoadAccel(qemuCaps, ctxt, VIR_DOMAIN_VIRT_NVMM) < 0) {
+        return -1;
+    }
     if (virQEMUCapsLoadAccel(qemuCaps, ctxt, VIR_DOMAIN_VIRT_QEMU) < 0)
         return -1;
 
@@ -4628,6 +4669,8 @@ virQEMUCapsLoadCache(virArch hostArch,
         virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_KVM);
     if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_HVF))
         virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_HVF);
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM))
+        virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_NVMM);
     virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_QEMU);
 
     if (virXPathBoolean("boolean(./kvmSupportsNesting)", ctxt) > 0)
@@ -4904,6 +4947,8 @@ virQEMUCapsFormatCache(virQEMUCaps *qemu
         virQEMUCapsFormatAccel(qemuCaps, &buf, VIR_DOMAIN_VIRT_KVM);
     if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_HVF))
         virQEMUCapsFormatAccel(qemuCaps, &buf, VIR_DOMAIN_VIRT_HVF);
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM))
+        virQEMUCapsFormatAccel(qemuCaps, &buf, VIR_DOMAIN_VIRT_NVMM);
     virQEMUCapsFormatAccel(qemuCaps, &buf, VIR_DOMAIN_VIRT_QEMU);
 
     for (i = 0; i < qemuCaps->ngicCapabilities; i++) {
@@ -5477,6 +5522,9 @@ virQEMUCapsGetVirtType(virQEMUCaps *qemu
     if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_HVF))
         return VIR_DOMAIN_VIRT_HVF;
 
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM))
+        return VIR_DOMAIN_VIRT_NVMM;
+
     if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_TCG))
         return VIR_DOMAIN_VIRT_QEMU;
 
@@ -5532,6 +5580,10 @@ virQEMUCapsInitQMPMonitor(virQEMUCaps *q
     if (virQEMUCapsProbeHVF(qemuCaps) < 0)
         return -1;
 
+    /* Some capabilities may differ depending on NVMM state */
+    if (virQEMUCapsProbeQMPNVMMState(qemuCaps, mon) < 0)
+        return -1;
+
     type = virQEMUCapsGetVirtType(qemuCaps);
     accel = virQEMUCapsGetAccel(qemuCaps, type);
 
@@ -5560,6 +5612,7 @@ virQEMUCapsInitQMPMonitor(virQEMUCaps *q
     if (virQEMUCapsProbeQMPSGXCapabilities(qemuCaps, mon) < 0)
         return -1;
 
+
     virQEMUCapsInitProcessCaps(qemuCaps);
 
     /* The following probes rely on other previously probed capabilities.
@@ -5660,6 +5713,16 @@ virQEMUCapsInitQMP(virQEMUCaps *qemuCaps
         virQEMUCapsInitQMPSingle(qemuCaps, libDir, runUid, runGid, true) < 0)
         return -1;
 
+    /*
+     * If NVMM was enabled during the first probe, we need to explicitly probe
+     * for TCG capabilities by asking the same binary again and turning KVM
+     * off.
+     */
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM) &&
+        virQEMUCapsGet(qemuCaps, QEMU_CAPS_TCG) &&
+        virQEMUCapsInitQMPSingle(qemuCaps, libDir, runUid, runGid, true) < 0)
+        return -1;
+
     return 0;
 }
 
@@ -5719,6 +5782,8 @@ virQEMUCapsNewForBinaryInternal(virArch
         virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_KVM);
     if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_HVF))
         virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_HVF);
+    if (virQEMUCapsGet(qemuCaps, QEMU_CAPS_NVMM))
+        virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_NVMM);
     virQEMUCapsInitHostCPUModel(qemuCaps, hostArch, VIR_DOMAIN_VIRT_QEMU);
 
     if (virQEMUCapsHaveAccel(qemuCaps)) {
@@ -6757,5 +6822,6 @@ virQEMUCapsStripMachineAliases(virQEMUCa
 {
     virQEMUCapsStripMachineAliasesForVirtType(qemuCaps, VIR_DOMAIN_VIRT_KVM);
     virQEMUCapsStripMachineAliasesForVirtType(qemuCaps, VIR_DOMAIN_VIRT_HVF);
+    virQEMUCapsStripMachineAliasesForVirtType(qemuCaps, VIR_DOMAIN_VIRT_NVMM);
     virQEMUCapsStripMachineAliasesForVirtType(qemuCaps, VIR_DOMAIN_VIRT_QEMU);
 }
