from conan import ConanFile

# Project class file
class TinyPom(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = (
                "qt/6.7.1", # In this version the Qt Multiedia works
                "qhotkey/1.4.2",
                #"glib/2.81.0"
                ) 
    generators = "cmake", "CMakeDeps", "CMakeToolchain"
    
    # Define the options you want to enable
    default_options = {
        # Build Qt as shared libraries
        "qt:shared": True,
        # Enable multimedia support (if supported by the recipe)
        "qt:qtmultimedia": True,
    }

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.options["qt"].qtmultimedia = True
        self.options["qt"].shared = True
        #self.options["qt"].with_gstreamer = True

    def imports(self):
        bin_folder = "../build/bin/" + str(self.settings.build_type);

        if self.settings.build_type == "Debug":
            suffix = "d"
        else:
            suffix = ""
            
        # Copy Qt runtime DLLs.
        qt_dlls = [
            "Qt6Core" + suffix + ".dll",
            "Qt6Gui" + suffix + ".dll",
            "Qt6Widgets" + suffix + ".dll",
            "Qt6Multimedia" + suffix + ".dll",
            "Qt6MultimediaWidgets" + suffix + ".dll",
            "Qt6Network" + suffix + ".dll"
        ]
        for dll in qt_dlls:
            self.copy(dll, bin_folder, "bin")

        # Copy Qt Multimedia Plugins (corrected path)
        #self.copy("*.dll", dst=bin_folder + "/plugins/multimedia", src="plugins/multimedia")            
        
        qt_multimedia_plugin_dll = [
            "ffmpegmediaplugin" + suffix + ".dll",
            "windowsmediaplugin" + suffix + ".dll"
        ]
        
        for dll in qt_multimedia_plugin_dll:
            self.copy(dll, dst=bin_folder + "/plugins/multimedia/", src="plugins/multimedia")
        
        # Copy the correct platform plugin
        self.copy("qwindows" + suffix + ".dll", dst=bin_folder + "/plugins/platforms/", src="plugins/platforms/")  # Windows
        self.copy("libqxcb.so", bin_folder + "/plugins/platforms/", "bin/plugins/platforms/")  # Linux (if applicable)
        