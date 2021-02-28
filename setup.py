from setuptools import setup, Extension, find_packages

qtools = Extension('qtools', 
        ['src/qtools_module.cc', 'src/base.cc','src/pricing.cc'],
        extra_compile_args=['-fPIC'],
        )
setup(name='qtools', ext_modules=[qtools])
