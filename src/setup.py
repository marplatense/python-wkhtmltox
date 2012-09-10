from distutils.core import setup, Extension
setup(name="wkhtmltox", version="0.1",
                ext_modules=[
                    Extension("wkhtmltox", ["wkhtmltox.c"], include_dirs=['/usr/include'])
                    ])
