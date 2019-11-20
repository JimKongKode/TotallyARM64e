# TotallyARM64e

TotallyARM64e is an experimental tool written by Jim Kong Kracks™️. It is a tool that tricks iOS into loading arm64 libraries into arm64e processes. In other words -- allows arm64 tweaks to run on arm64e.

# How it works

This tool works by spoofing ARM64e as the architecture in a binary's `mach_header_64` or `fat_header`. One single byte has to be changed, and it will be loaded into the process. 

# Why Apple is an asshole

PAC
