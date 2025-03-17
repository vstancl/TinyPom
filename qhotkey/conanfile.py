from conans import ConanFile, CMake, tools
import os
import shutil

class QHotkeyConan(ConanFile):
    name = "qhotkey"
    version = "1.4.2"
    license = "BSD-3-Clause"
    author = "Original: Skycoder42"
    url = "https://github.com/Skycoder42/QHotkey"
    description = "A global shortcut/hotkey library for Qt"
    topics = ("qt", "hotkey", "shortcut", "global-hotkey")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake_find_package"  # Changed to cmake_find_package
    exports_sources = "CMakeLists.txt"
    requires = "qt/6.7.1"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        git = tools.Git(folder="source")
        git.clone("https://github.com/Skycoder42/QHotkey.git")
        
        # Copy the CMakeLists.txt to the source directory using shutil
        shutil.copy(os.path.join(self.source_folder, "CMakeLists.txt"), 
                    os.path.join(self.source_folder, "source", "CMakeLists.txt"))

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["CMAKE_INSTALL_PREFIX"] = self.package_folder
        #cmake.definitions["CMAKE_PREFIX_PATH"] = self.build_folder
        cmake.definitions["QT_DEFAULT_MAJOR_VERSION"] = "6"
        cmake.configure(source_dir="source")
        #cmake = CMake(self)
        # Add these variables to help find Qt6
        
#        with tools.chdir("source"):
#            cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.definitions["QT_DEFAULT_MAJOR_VERSION"] = "6"
        cmake.configure(source_dir="source")       
        #with tools.chdir("source"):
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        #self.copy("LICENSE", dst="licenses", src="source")
        #self.copy("*.h", dst="include/QHotkey", src="source/QHotkey")
        #self.copy("QHotkey", dst="include/QHotkey", src="source/QHotkey")
        
        # Debug output - list what's in the package directory
        self.output.info("Files in include directory:")
        for root, dirs, files in os.walk(os.path.join(self.package_folder, "include")):
            for file in files:
                self.output.info(os.path.join(root, file))
                

    def package_info(self):
        self.cpp_info.libs = ["QHotkey"]
        self.cpp_info.includedirs = ["include", "include/QHotkey"]
        self.cpp_info.builddirs = [self.package_folder]