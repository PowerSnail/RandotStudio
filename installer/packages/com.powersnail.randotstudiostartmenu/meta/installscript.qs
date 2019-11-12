function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    if (systemInfo.productType === "windows") {
        component.addOperation(
            "CreateShortcut", 
            "@TargetDir@/RandotStudio.exe", 
            "@StartMenuDir@/RandotStudio.lnk",
            "workingDirectory=@TargetDir@", 
            "iconPath=@TargetDir@/RandotStudio.exe",
            "iconId=0", 
            "description=Open RandotStudio");
        component.addOperation(
            "CreateShortcut", 
            "@TargetDir@/maintenancetool.exe", 
            "@StartMenuDir@/Uninstall RandotStudio.lnk",
            "workingDirectory=@TargetDir@", 
            "iconPath=@TargetDir@/maintenancetool.exe",
            "iconId=0", 
            "description=Uninstall or modify RandotStudio");
    }
}