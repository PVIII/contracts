from conans import ConanFile, CMake

class Contracts(ConanFile):
    name = "contracts"
    version = "0.1"
    url = "https://github.com/PVIII/contracts.git"
    license = "MIT"
    description = "Macro magic to enable a subset of the contract programming proposal."
    settings = "os", "compiler", "arch", "build_type"
    generators = "cmake"
    build_requires = "cpputils/1.0.1@pviii/testing"
    exports_sources = "include/**", "CMakeLists.txt", "test/CMakeLists.txt", "test/*.cpp"
    no_copy_source = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.copy("*.hpp")
        self.copy("CMakeLists.txt")

    def package_id(self):
        self.info.header_only()
