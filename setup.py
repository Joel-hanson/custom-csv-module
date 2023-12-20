from distutils.core import Extension, setup

module = Extension("customcsv", sources=["customcsv.c"])

setup(
    name="customcsv",
    version="1.0",
    description="Custom CSV reader",
    ext_modules=[module],
)
