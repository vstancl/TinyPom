from conan import ConanFile

class TinyPom(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "qt/6.8.2@common+conan-recipes/stable",
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
