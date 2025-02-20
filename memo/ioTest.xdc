
#
############## clock define##################
create_clock -period 20.000 [get_ports {sysClk50m}]
set_property PACKAGE_PIN W19 [get_ports {sysClk50m}]
#set_property IOSTANDARD DIFF_SSTL15 [get_ports {sysClk}]
set_property IOSTANDARD LVCMOS25 [get_ports {sysClk50m}]


# switchK1, resetBN
set_property PACKAGE_PIN H13 [get_ports resetN]
set_property IOSTANDARD LVCMOS25 [get_ports resetN]
# ledV1
set_property PACKAGE_PIN K13 [get_ports {ledV1}]
set_property IOSTANDARD LVCMOS25 [get_ports {ledV1}]
# ledV3
set_property PACKAGE_PIN H14 [get_ports {ledV3}]
set_property IOSTANDARD LVCMOS25 [get_ports {ledV3}]
# ledV4
set_property PACKAGE_PIN J14 [get_ports {ledV4}]
set_property IOSTANDARD LVCMOS25 [get_ports {ledV4}]

# ===================================================
# ioA0
set_property PACKAGE_PIN T4 [get_ports {ioA[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[0]}]

# ioA1
set_property PACKAGE_PIN U3 [get_ports {ioA[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[1]}]

# ioA2
set_property PACKAGE_PIN V4 [get_ports {ioA[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[2]}]

# ioA3
set_property PACKAGE_PIN Y3 [get_ports {ioA[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[3]}]

# ioA4
set_property PACKAGE_PIN R2 [get_ports {ioA[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[4]}]

# ioA5
set_property PACKAGE_PIN U1 [get_ports {ioA[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[5]}]

# ioA6
set_property PACKAGE_PIN W1 [get_ports {ioA[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[6]}]

# ioA7
set_property PACKAGE_PIN Y1 [get_ports {ioA[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[7]}]

# ioA8
set_property PACKAGE_PIN AB1 [get_ports {ioA[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[8]}]

# ioA9
set_property PACKAGE_PIN AB2 [get_ports {ioA[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[9]}]

# ioA10
set_property PACKAGE_PIN W5 [get_ports {ioA[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[10]}]

# ioA11
set_property PACKAGE_PIN R6 [get_ports {ioA[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[11]}]

# ioA12
set_property PACKAGE_PIN W6 [get_ports {ioA[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[12]}]

# ioA13
set_property PACKAGE_PIN Y6 [get_ports {ioA[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[13]}]

# ioA14
set_property PACKAGE_PIN W7 [get_ports {ioA[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[14]}]

# ioA15
set_property PACKAGE_PIN V8 [get_ports {ioA[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[15]}]

# ioA16
set_property PACKAGE_PIN AB5 [get_ports {ioA[16]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[16]}]

# ioA17
set_property PACKAGE_PIN AB6 [get_ports {ioA[17]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[17]}]

# ioA18
set_property PACKAGE_PIN AB7 [get_ports {ioA[18]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[18]}]

# ioA19
set_property PACKAGE_PIN AB8 [get_ports {ioA[19]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[19]}]

# ioA20
set_property PACKAGE_PIN AB10 [get_ports {ioA[20]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[20]}]

# ioA21
set_property PACKAGE_PIN AB11 [get_ports {ioA[21]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[21]}]

# ioA22
set_property PACKAGE_PIN AA13 [get_ports {ioA[22]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[22]}]

# ioA23
set_property PACKAGE_PIN AA9 [get_ports {ioA[23]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[23]}]

# ioA24
set_property PACKAGE_PIN Y8 [get_ports {ioA[24]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[24]}]

# ioA25
set_property PACKAGE_PIN Y11 [get_ports {ioA[25]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[25]}]

# ioA26
set_property PACKAGE_PIN V9 [get_ports {ioA[26]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[26]}]

# ioA27
set_property PACKAGE_PIN W10 [get_ports {ioA[27]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[27]}]

# ioA28
set_property PACKAGE_PIN W12 [get_ports {ioA[28]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[28]}]

# ioA29
set_property PACKAGE_PIN Y14 [get_ports {ioA[29]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[29]}]

# ioA30
set_property PACKAGE_PIN W14 [get_ports {ioA[30]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[30]}]

# ioA31
set_property PACKAGE_PIN W15 [get_ports {ioA[31]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[31]}]

# ioA32
set_property PACKAGE_PIN AB15 [get_ports {ioA[32]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[32]}]

# ioA33
set_property PACKAGE_PIN AB16 [get_ports {ioA[33]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[33]}]

# ioA34
set_property PACKAGE_PIN AB17 [get_ports {ioA[34]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[34]}]

# ioA35
set_property PACKAGE_PIN AB18 [get_ports {ioA[35]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[35]}]

# ioA36
set_property PACKAGE_PIN R14 [get_ports {ioA[36]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[36]}]

# ioA37
set_property PACKAGE_PIN V15 [get_ports {ioA[37]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[37]}]

# ioA38
set_property PACKAGE_PIN Y17 [get_ports {ioA[38]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[38]}]

# ioA39
set_property PACKAGE_PIN AA19 [get_ports {ioA[39]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[39]}]

# ioA40
set_property PACKAGE_PIN T5 [get_ports {ioA[40]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[40]}]

# ioA41
set_property PACKAGE_PIN U5 [get_ports {ioA[41]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[41]}]

# ioA42
set_property PACKAGE_PIN V3 [get_ports {ioA[42]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[42]}]

# ioA43
set_property PACKAGE_PIN W4 [get_ports {ioA[43]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[43]}]

# ioA44
set_property PACKAGE_PIN AA3 [get_ports {ioA[44]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[44]}]

# ioA45
set_property PACKAGE_PIN T1 [get_ports {ioA[45]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[45]}]

# ioA46
set_property PACKAGE_PIN U2 [get_ports {ioA[46]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[46]}]

# ioA47
set_property PACKAGE_PIN W2 [get_ports {ioA[47]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[47]}]

# ioA48
set_property PACKAGE_PIN AA1 [get_ports {ioA[48]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[48]}]

# ioA49
set_property PACKAGE_PIN Y2 [get_ports {ioA[49]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[49]}]

# ioA50
set_property PACKAGE_PIN AB3 [get_ports {ioA[50]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[50]}]

# ioA51
set_property PACKAGE_PIN Y4 [get_ports {ioA[51]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[51]}]

# ioA52
set_property PACKAGE_PIN V5 [get_ports {ioA[52]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[52]}]

# ioA53
set_property PACKAGE_PIN T6 [get_ports {ioA[53]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[53]}]

# ioA54
set_property PACKAGE_PIN Y7 [get_ports {ioA[54]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[54]}]

# ioA55
set_property PACKAGE_PIN V7 [get_ports {ioA[55]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[55]}]

# ioA56
set_property PACKAGE_PIN AA4 [get_ports {ioA[56]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[56]}]

# ioA57
set_property PACKAGE_PIN AA5 [get_ports {ioA[57]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[57]}]

# ioA58
set_property PACKAGE_PIN AA6 [get_ports {ioA[58]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[58]}]

# ioA59
set_property PACKAGE_PIN AA8 [get_ports {ioA[59]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[59]}]

# ioA60
set_property PACKAGE_PIN AA10 [get_ports {ioA[60]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioA[60]}]

# ================================================================

# ioB0
set_property PACKAGE_PIN G15 [get_ports {ioB[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[0]}]

# ioB1
set_property PACKAGE_PIN G18 [get_ports {ioB[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[1]}]

# ioB2
set_property PACKAGE_PIN H15 [get_ports {ioB[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[2]}]

# ioB3
set_property PACKAGE_PIN H17 [get_ports {ioB[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[3]}]

# ioB4
set_property PACKAGE_PIN J17 [get_ports {ioB[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[4]}]

# ioB5
set_property PACKAGE_PIN J19 [get_ports {ioB[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[5]}]

# ioB6
set_property PACKAGE_PIN J15 [get_ports {ioB[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[6]}]

# ioB7
set_property PACKAGE_PIN K14 [get_ports {ioB[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[7]}]

# ioB8
set_property PACKAGE_PIN L15 [get_ports {ioB[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[8]}]

# ioB9
set_property PACKAGE_PIN L19 [get_ports {ioB[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[9]}]

# ioB10
set_property PACKAGE_PIN M18 [get_ports {ioB[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[10]}]

# ioB11
set_property PACKAGE_PIN H20 [get_ports {ioB[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[11]}]

# ioB12
set_property PACKAGE_PIN J21 [get_ports {ioB[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[12]}]

# ioB13
set_property PACKAGE_PIN K21 [get_ports {ioB[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[13]}]

# ioB14
set_property PACKAGE_PIN L21 [get_ports {ioB[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[14]}]

# ioB15
set_property PACKAGE_PIN M21 [get_ports {ioB[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[15]}]

# ioB16
set_property PACKAGE_PIN K19 [get_ports {ioB[16]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[16]}]

# ioB17
set_property PACKAGE_PIN L18 [get_ports {ioB[17]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[17]}]

# ioB18
set_property PACKAGE_PIN N17 [get_ports {ioB[18]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[18]}]

# ioB19
set_property PACKAGE_PIN N18 [get_ports {ioB[19]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[19]}]

# ioB20
set_property PACKAGE_PIN N20 [get_ports {ioB[20]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[20]}]

# ioB21
set_property PACKAGE_PIN P17 [get_ports {ioB[21]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[21]}]

# ioB22
set_property PACKAGE_PIN P16 [get_ports {ioB[22]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[22]}]

# ioB23
set_property PACKAGE_PIN T18 [get_ports {ioB[23]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[23]}]

# ioB24
set_property PACKAGE_PIN U18 [get_ports {ioB[24]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[24]}]

# ioB25
set_property PACKAGE_PIN R17 [get_ports {ioB[25]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[25]}]

# ioB26
set_property PACKAGE_PIN P15 [get_ports {ioB[26]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[26]}]

# ioB27
set_property PACKAGE_PIN R16 [get_ports {ioB[27]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[27]}]

# ioB28
set_property PACKAGE_PIN V17 [get_ports {ioB[28]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[28]}]

# ioB29
set_property PACKAGE_PIN U17 [get_ports {ioB[29]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[29]}]

# ioB30
set_property PACKAGE_PIN V19 [get_ports {ioB[30]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[30]}]

# ioB31
set_property PACKAGE_PIN Y18 [get_ports {ioB[31]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[31]}]

# ioB32
set_property PACKAGE_PIN U21 [get_ports {ioB[32]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[32]}]

# ioB33
set_property PACKAGE_PIN W20 [get_ports {ioB[33]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[33]}]

# ioB34
set_property PACKAGE_PIN W21 [get_ports {ioB[34]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[34]}]

# ioB35
set_property PACKAGE_PIN Y21 [get_ports {ioB[35]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[35]}]

# ioB36
set_property PACKAGE_PIN AA21 [get_ports {ioB[36]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[36]}]

# ioB37
set_property PACKAGE_PIN AA20 [get_ports {ioB[37]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[37]}]

# ioB38
set_property PACKAGE_PIN G16 [get_ports {ioB[38]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[38]}]

# ioB39
set_property PACKAGE_PIN H19 [get_ports {ioB[39]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[39]}]

# ioB40
set_property PACKAGE_PIN H18 [get_ports {ioB[40]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[40]}]

# ioB41
set_property PACKAGE_PIN K16 [get_ports {ioB[41]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[41]}]

# ioB42
set_property PACKAGE_PIN K18 [get_ports {ioB[42]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[42]}]

# ioB43
set_property PACKAGE_PIN J16 [get_ports {ioB[43]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[43]}]

# ioB44
set_property PACKAGE_PIN J20 [get_ports {ioB[44]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[44]}]

# ioB45
set_property PACKAGE_PIN L20 [get_ports {ioB[45]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[45]}]

# ioB46
set_property PACKAGE_PIN M20 [get_ports {ioB[46]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[46]}]

# ioB47
set_property PACKAGE_PIN L16 [get_ports {ioB[47]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[47]}]

# ioB48
set_property PACKAGE_PIN N15 [get_ports {ioB[48]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[48]}]

# ioB49
set_property PACKAGE_PIN H22 [get_ports {ioB[49]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[49]}]

# ioB50
set_property PACKAGE_PIN J22 [get_ports {ioB[50]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[50]}]

# ioB51
set_property PACKAGE_PIN K22 [get_ports {ioB[51]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[51]}]

# ioB52
set_property PACKAGE_PIN M22 [get_ports {ioB[52]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[52]}]

# ioB53
set_property PACKAGE_PIN N22 [get_ports {ioB[53]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[53]}]

# ioB54
set_property PACKAGE_PIN K17 [get_ports {ioB[54]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[54]}]

# ioB55
set_property PACKAGE_PIN M17 [get_ports {ioB[55]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[55]}]

# ioB56
set_property PACKAGE_PIN M16 [get_ports {ioB[56]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[56]}]

# ioB57
set_property PACKAGE_PIN N14 [get_ports {ioB[57]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[57]}]

# ioB58
set_property PACKAGE_PIN N19 [get_ports {ioB[58]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[58]}]

# ioB59
set_property PACKAGE_PIN R19 [get_ports {ioB[59]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[59]}]

# ioB60
set_property PACKAGE_PIN P19 [get_ports {ioB[60]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[60]}]

# ioB61
set_property PACKAGE_PIN T21 [get_ports {ioB[61]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[61]}]

# ioB62
set_property PACKAGE_PIN T20 [get_ports {ioB[62]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[62]}]

# ioB63
set_property PACKAGE_PIN R18 [get_ports {ioB[63]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[63]}]

# ioB64
set_property PACKAGE_PIN T16 [get_ports {ioB[64]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[64]}]

# ioB65
set_property PACKAGE_PIN U16 [get_ports {ioB[65]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[65]}]

# ioB66
set_property PACKAGE_PIN V18 [get_ports {ioB[66]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[66]}]

# ioB67
set_property PACKAGE_PIN U20 [get_ports {ioB[67]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ioB[67]}]

# ================================================================

# RS485RO
set_property PACKAGE_PIN W17 [get_ports {RS485RO}]
set_property IOSTANDARD LVCMOS25 [get_ports {RS485RO}]

# RS485_DI
set_property PACKAGE_PIN AB20 [get_ports {RS485DI}]
set_property IOSTANDARD LVCMOS25 [get_ports {RS485DI}]


# ================================================================
# DF0  L8
set_property PACKAGE_PIN C13 [get_ports {DFP[0]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[0]}]
set_property PACKAGE_PIN B13 [get_ports {DFN[0]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[0]}]

# DF1 L6
set_property PACKAGE_PIN D14 [get_ports {DFP[1]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[1]}]
set_property PACKAGE_PIN D15 [get_ports {DFN[1]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[1]}]

# DF2 L10
set_property PACKAGE_PIN A13 [get_ports {DFP[2]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[2]}]
set_property PACKAGE_PIN A14 [get_ports {DFN[2]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[2]}]

# DF3 L9
set_property PACKAGE_PIN A15 [get_ports {DFP[3]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[3]}]
set_property PACKAGE_PIN A16 [get_ports {DFN[3]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[3]}]

# DF4 L1
set_property PACKAGE_PIN F13 [get_ports {DFP[4]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[4]}]
set_property PACKAGE_PIN F14 [get_ports {DFN[4]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[4]}]

# DF5 L3
set_property PACKAGE_PIN C14 [get_ports {DFP[5]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[5]}]
set_property PACKAGE_PIN C15 [get_ports {DFN[5]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[5]}]

# DF6 L15
set_property PACKAGE_PIN F18 [get_ports {DFP[6]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[6]}]
set_property PACKAGE_PIN E18 [get_ports {DFN[6]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[6]}]

# DF7 L7
set_property PACKAGE_PIN B15 [get_ports {DFP[7]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[7]}]
set_property PACKAGE_PIN B15 [get_ports {DFN[7]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[7]}]

# DF8 L11
set_property PACKAGE_PIN B17 [get_ports {DFP[8]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[8]}]
set_property PACKAGE_PIN B16 [get_ports {DFN[8]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[8]}]

# DF9 L5
set_property PACKAGE_PIN E16 [get_ports {DFP[9]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[9]}]
set_property PACKAGE_PIN D16 [get_ports {DFN[9]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[9]}]

# DF10 L13
set_property PACKAGE_PIN C18 [get_ports {DFP[10]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[10]}]
set_property PACKAGE_PIN C19 [get_ports {DFN[10]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[10]}]

# DF11 L19
set_property PACKAGE_PIN D20 [get_ports {DFP[11]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[11]}]
set_property PACKAGE_PIN C20 [get_ports {DFN[11]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[11]}]

# DF12 L17
set_property PACKAGE_PIN A18 [get_ports {DFP[12]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[12]}]
set_property PACKAGE_PIN A19 [get_ports {DFN[12]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[12]}]

# DF13 L16
set_property PACKAGE_PIN B20 [get_ports {DFP[13]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[13]}]
set_property PACKAGE_PIN A20 [get_ports {DFN[13]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[13]}]

# DF14 L21
set_property PACKAGE_PIN B21 [get_ports {DFP[14]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[14]}]
set_property PACKAGE_PIN A21 [get_ports {DFN[14]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[14]}]

# DF15 L23
set_property PACKAGE_PIN E21 [get_ports {DFP[15]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[15]}]
set_property PACKAGE_PIN D21 [get_ports {DFN[15]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[15]}]

# DF16 L14
set_property PACKAGE_PIN E19 [get_ports {DFP[16]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[16]}]
set_property PACKAGE_PIN D19 [get_ports {DFN[16]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[16]}]

# DF17 L18
set_property PACKAGE_PIN F19 [get_ports {DFP[17]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[17]}]
set_property PACKAGE_PIN F20 [get_ports {DFN[17]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[17]}]

# DF18 L20
set_property PACKAGE_PIN C22 [get_ports {DFP[18]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[18]}]
set_property PACKAGE_PIN B22 [get_ports {DFN[18]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[18]}]

# DF19 L22
set_property PACKAGE_PIN E22 [get_ports {DFP[19]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFP[19]}]
set_property PACKAGE_PIN D22 [get_ports {DFN[19]}]
set_property IOSTANDARD LVDS_25 [get_ports {DFN[19]}]


##############outA define##################
#set_property PACKAGE_PIN H20 [get_ports {outA[0]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {outA[0]}]

#set_property PACKAGE_PIN G17 [get_ports {outA[1]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {outA[1]}]

#set_property PACKAGE_PIN E22 [get_ports {outA[2]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {outA[2]}]

#set_property PACKAGE_PIN D14 [get_ports {outA[3]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {outA[3]}]



##############hwoutA ###################
#SSP_WG_CLK_P BC5 H20
#set_property PACKAGE_PIN M21 [get_ports {D40M}]
#set_property IOSTANDARD LVCMOS25 [get_ports {D40M}]



##############hwoutA ###################
#SSP_WG_CLK_P BC5 H20
#set_property PACKAGE_PIN H20 [get_ports {outA[0]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {outA[0]}]
#set_property IOSTANDARD LVDS_25 [get_ports {outA[0]}]




#############SPI Configurate Setting##################
set_property BITSTREAM.CONFIG.SPI_BUSWIDTH 4 [current_design]
set_property CONFIG_MODE SPIx4 [current_design]
set_property BITSTREAM.CONFIG.CONFIGRATE 50 [current_design]