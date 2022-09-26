#!/usr/bin/env python

import os, re, string, subprocess, sys, importlib
Import('env')
#
#------------------------------------------------------------------------------
# last two components of the path. Ex: /not/this/but/THIS/AND_THIS
#                                      "AND_THIS" is usually "src"
#------------------------------------------------------------------------------
package       = 'gaasAna'
muse_work_dir = os.getenv('MUSE_WORK_DIR');

x = subprocess.call(muse_work_dir+'/'+package+'/scripts/build_config '+'package',shell=True)
gaasAna_env = env.Clone()
#------------------------------------------------------------------------------
exec(open(muse_work_dir+"/site_scons/stntuple_site_init.py").read())
#
sys.path.append(muse_work_dir+'/site_scons')

from stntuple_helper import *
#------------------------------------------------------------------------------
# last two components of the path. Ex: /not/this/but/THIS/AND_THIS
#                                      "AND_THIS" is usually "src"
#------------------------------------------------------------------------------
w   = os.getenv('MUSE_BUILD_DIR').split('/')
w1  = w[0:len(w)-2]
dir = '/'.join(w1)

gaasAna_env['CPPPATH' ].append('-I'+dir+'/include');
gaasAna_env['CXXFLAGS'].append('-I'+dir+'/include');
#------------------------------------------------------------------------------
# done
#------------------------------------------------------------------------------
gaasAna_env.Append(BUILDERS = {'StntupleCodegen'  : stntuple_codegen })
gaasAna_env.Append(BUILDERS = {'StntupleRootCint' : stntuple_rootcint})

Export('gaasAna_env')
Export('stntuple_helper')
