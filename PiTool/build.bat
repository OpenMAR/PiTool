qmake -tp vc PiTool.pro
msbuild "%1" /p:Platform=%2;Configuration=%3
