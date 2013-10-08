VERSION = '0.0.1'
APPMNAME = 'TextMiningTool'

subdirs = ['src']

def options(opt):
    opt.load('compiler_cxx unittest_gtest')


def configure(conf):
    conf.load('compiler_cxx unittest_gtest')
    conf.env.append_value('CXXFLAGS', ['-msse4.2'])

def build(bld):
    bld.recurse(subdirs)
