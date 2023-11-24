
-call_properties+={"name(printk)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"name(debugtrace_printk)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"name(panic)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"macro(^domain_crash$)", {"pointee_write(2..=never)", "taken()"}}
-call_properties+={"macro(^(g?d|mm_)?printk$)", {"pointee_write(2..=never)", "taken()"}}
-call_properties+={"macro(^guest_bug_on_failed$)", {"pointee_write(1=never)", "taken()"}}
-call_properties+={"macro(^spin_lock_init_prof$)", {"pointee_write(2=never)", "taken()"}}
-call_properties+={"macro(^sched_test_func$)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"macro(^dev_(info|warn)$)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"macro(^PAGING_DEBUG$)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"macro(^ACPI_(WARNING|ERROR|INFO)$)", {"pointee_write(1..=never)", "taken()"}}
-call_properties+={"name(fdt_get_property_by_offset_)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(read_atomic_size)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(device_tree_get_reg)", {"pointee_write(4..=always)", "pointee_read(4..=never)", "taken()"}}
-call_properties+={"name(dt_get_range)", {"pointee_write(3..=always)", "pointee_read(3..=never)", "taken()"}}
-call_properties+={"name(parse_static_mem_prop)", {"pointee_write(2..=always)", "pointee_read(2..=never)", "taken()"}}
-call_properties+={"name(get_ttbr_and_gran_64bit)", {"pointee_write(1..2=always)", "pointee_read(1..2=never)", "taken()"}}
-call_properties+={"name(hvm_emulate_init_once)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(__vmread)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(hvm_pci_decode_addr)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(vpci_mmcfg_decode_addr)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(x86emul_decode)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(unmap_grant_ref)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(arm_smmu_cmdq_build_cmd)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(pci_size_mem_bar)", {"pointee_write(4=always)", "pointee_read(4=never)", "taken()"}}
-call_properties+={"name(_hvm_read_entry)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(hvm_map_guest_frame_rw)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(guest_cpuid)", {"pointee_write(4=always)", "pointee_read(4=never)", "taken()"}}
-call_properties+={"name(epte_get_entry_emt)", {"pointee_write(5=always)", "pointee_read(5=never)", "taken()"}}
-call_properties+={"name(mcheck_mca_logout)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(set_field_in_reg_u32)", {"pointee_write(5=always)", "pointee_read(5=never)", "taken()"}}
-call_properties+={"name(alloc_affinity_masks)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(xasprintf)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(find_non_smt)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(call_rcu)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(getdomaininfo)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"^MAPPING_(INSERT|SEARCH)\\(.*$", {"pointee_write(2..=always)", "pointee_read(2..=never)", "taken()"}}
-call_properties+={"name(FormatDec)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(FormatHex)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(p2m_get_ioreq_server)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(elf_memset_unchecked)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(set_iommu_pte_present)", {"pointee_write(7=always)", "pointee_read(7=never)", "taken()"}}
-call_properties+={"name(clear_iommu_pte_present)", {"pointee_write(4=always)", "pointee_read(4=never)", "taken()"}}
-call_properties+={"name(vcpu_runstate_get)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(va_start)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(sgi_target_init)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(get_hw_residencies)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(x86_cpu_policy_to_featureset)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"^simple_strtou?ll?\\(.*$", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(msi_compose_msg)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(print_tainted)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(get_hvm_registers)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(x86_insn_modrm)", {"pointee_write(2..3=always)", "pointee_read(2..3=never)", "taken()"}}
-call_properties+={"name(cpuid_count_leaf)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-call_properties+={"name(rcu_lock_remote_domain_by_id)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(cpuid_count)", {"pointee_write(3..=always)", "pointee_read(3..=never)", "taken()"}}
-call_properties+={"name(efi_boot_mem_unused)", {"pointee_write(1..=always)", "pointee_read(1..=never)", "taken()"}}
-call_properties+={"name(collect_time_info)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-call_properties+={"name(setup_xstate_comp)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"name(map_domain_gfn)", {"pointee_read(5=never)", "taken()"}}
-call_properties+={"name(fdt_getprop)", {"pointee_read(4=never)", "taken()"}}
-call_properties+={"name(fdt_get_name)", {"pointee_read(3=never)", "taken()"}}
-call_properties+={"name(fdt_get_property)", {"pointee_read(4=never)", "taken()"}}
-call_properties+={"name(pci_get_host_bridge_segment)", {"pointee_read(2=never)", "taken()"}}
-call_properties+={"name(dt_get_property)", {"pointee_read(3=never)", "taken()"}}
-call_properties+={"name(dt_property_read_u32)", {"pointee_read(3=never)", "taken()"}}
-call_properties+={"name(dt_device_get_paddr)", {"pointee_read(3..4=never)", "taken()"}}
-call_properties+={"name(get_evtchn_dt_property)", {"pointee_write(2..3=maybe)", "pointee_read(2..3=never)", "taken()"}}
-call_properties+={"name(setup_chosen_node)", {"pointee_write(2..3=maybe)", "pointee_read(2..3=never)", "taken()"}}
-call_properties+={"name(queue_remove_raw)", {"pointee_read(2=never)", "taken()"}}
-call_properties+={"macro(^memset$)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"macro(^va_start$)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"macro(^memcmp$)", {"pointee_write(1..2=never)", "taken()"}}
-call_properties+={"macro(^memcpy$)", {"pointee_write(1=always&&2..=never)", "pointee_read(1=never&&2..=always)", "taken()"}}
-call_properties+={"name(get_cpu_info)",{pure}}
-call_properties+={"name(pdx_to_pfn)",{pure}}
-call_properties+={"name(is_pci_passthrough_enabled)",{const}}
-call_properties+={"name(get_cycles)", {"noeffect"}}
-call_properties+={"name(msi_gflags)",{const}}
-call_properties+={"name(hvm_save_size)",{pure}}
-call_properties+={"name(cpu_has)",{pure}}
-call_properties+={"name(boot_cpu_has)",{pure}}
-call_properties+={"name(get_cpu_info)",{pure}}
-call_properties+={"name(put_pte_flags)",{const}}
-call_properties+={"name(is_pv_vcpu)",{pure}}

-doc_begin="Property inferred as a consequence of the semantics of device_tree_get_reg"
-call_properties+={"name(acquire_static_memory_bank)", {"pointee_write(4..=always)", "pointee_read(4..=never)", "taken()"}}
-doc_end

-doc_begin="Property inferred as a consequence of the semantics of dt_set_cell"
-call_properties+={"name(set_interrupt)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-doc_end

-doc_begin="Property inferred as a consequence of the semantics of __p2m_get_mem_access"
-call_properties+={"name(p2m_get_mem_access)", {"pointee_write(3=always)", "pointee_read(3=never)", "taken()"}}
-doc_end

-doc_begin="This function has alternative definitions with props {write=always,read=never} and {write=never,read=never}"
-call_properties+={"name(alloc_cpumask_var)", {"pointee_write(1=maybe)", "pointee_read(1=never)", "taken()"}}
-doc_end

-doc_begin="Property inferred as a consequence of the semantics of alloc_cpumask_var"
-call_properties+={"name(xenctl_bitmap_to_cpumask)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-doc_end

-doc_begin="The call to bitmap_and causes the pointee of dstp to be always written"
-call_properties+={"^cpumask_(and|andnot|clear|copy|complement).*$", {"pointee_write(1=always)", "pointee_read(1=never)" "taken()"}}
-call_properties+={"^bitmap_(andnot|complement|fill).*$", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-doc_end

-doc_begin="The .*copy_(to|from).* helpers all have a memcpy-like expectation that the destination is a copy of the source.
Furthermore, their uses do initialize the involved variables as needed by futher uses in the caller."
-call_properties+={"macro(^(__)?(raw_)?copy_from_(paddr|guest|compat)(_offset)?$)", {"pointee_write(1=always)", "pointee_read(1=never)", "taken()"}}
-call_properties+={"macro(^(__)?copy_to_(guest|compat)(_offset)?$)", {"pointee_write(2=always)", "pointee_read(2=never)", "taken()"}}
-doc_end

-doc_begin="Functions generated by build_atomic_read cannot be considered pure
since the input pointer is volatile, but they do not produce any persistent side
effect."
-call_properties+={"^read_u(8|16|32|64|int)_atomic.*$", {noeffect}}
-doc_end

-doc_begin="Functions generated by TYPE_SAFE are const."
-call_properties+={"^(mfn|gfn|pfn)_x\\(.*$",{const}}
-call_properties+={"^_(mfn|gfn|pfn)\\(.*$",{const}}
-doc_end
