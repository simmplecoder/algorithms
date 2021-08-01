import os

from conans import ConanFile, tools


class SortingConan(ConanFile):
    name = "sorting"
    version = "0.0.1"
    license = "MIT"
    author = "Olzhas Zhumabek <anonymous.from.applecity@gmail.com>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Algorithms for sorting ranges"
    topics = ("C++17", "sorting")
    exports_sources = "include/sorting/*"
    no_copy_source = True
    # No settings/options are necessary, this is header only

    def package(self):
        # self.copy("*.hpp", dst="include/sorting", src="/home/shino/work/algorithms/include/sorting")
        self.copy("*.hpp", dst="include/sorting", src="include/sorting")

    def package_id(self):
        self.info.header_only()
