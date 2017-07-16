#!/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/external/slc6_amd64_gcc530/bin/python

import os
try:
    import ROOT as r
    r.gROOT.SetBatch(True)
except:
    import sys
    if "LD_LIBRARY_PATH" not in os.environ:
        os.environ["ROOTSYS"]="/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed4/"
        os.environ["PATH"]="/cvmfs/cms.cern.ch/share/overrides/bin:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/bin/slc6_amd64_gcc530:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/external/slc6_amd64_gcc530/bin:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/llvm/3.7.1-giojec/bin:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/bin:/cvmfs/cms.cern.ch/common:/cvmfs/cms.cern.ch/bin:~/software/bin:~/login/bin:/usr/local/bin:~/software/bin:~/login/bin:/usr/local/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin"
        os.environ["LD_LIBRARY_PATH"]="/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/biglib/slc6_amd64_gcc530:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/lib/slc6_amd64_gcc530:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/external/slc6_amd64_gcc530/lib:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/llvm/3.7.1-giojec/lib64:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/lib64:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/lib"
        os.environ["PYTHONPATH"]="/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/python:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/lib/slc6_amd64_gcc530:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/coral/CORAL_2_3_21-ikhhed6/slc6_amd64_gcc530/python:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/coral/CORAL_2_3_21-ikhhed6/slc6_amd64_gcc530/lib:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/pyqt/4.8.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/python-ldap/2.4.10-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-scipy/0.8.0-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pandas/0.17.1-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-matplotlib/1.2.1-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/yoda/1.5.5-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/sip/4.11.2-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/llvm/3.7.1-giojec/lib64/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-sqlalchemy/0.8.2-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pycurl/7.43.0/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pytz/2014.7-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-python-dateutil/1.5-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-numpy/1.9.2-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-lint/0.25.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-ipython/3.1.0-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-dxr/1.0-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-cx-oracle/5.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-cjson/1.0.5-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/frontier_client/2.8.19-ikhhed/python/lib:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/das_client/v02.17.04/bin:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/cython/0.22-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/rivet/2.4.0-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed4/lib:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/pyminuit2/0.0.1-ikhhed4/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-six/1.9.0-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-schema/0.3.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-requests/2.5.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-PyYAML/3.11-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pyparsing/2.0.3-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pygithub/1.23.0-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-prettytable/0.7.2-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-networkx/1.8.1-giojec/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pysqlite/2.8.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-pygments/1.6-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-parsimonious/0.6.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-ordereddict/1.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-markupsafe/0.23-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-jinja/2.7.2-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-futures/2.2.0-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-docopt/0.6.2-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/py2-dablooms/0.9.1-ikhhed/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/professor/1.4.0-ikhhed4/lib/python2.7/site-packages:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/dbs-client/DBS_2_1_9-ikhhed/lib:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/dbs-client/DBS_2_1_9-ikhhed/lib/DBSAPI:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/cvs2git/5419-ikhhed/lib"
        os.environ["ROOT_INCLUDE_PATH"]="/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/src:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/coral/CORAL_2_3_21-ikhhed6/include/LCG:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/mctester/1.25.0a-ikhhed4/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed4/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/QtDesigner:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/tauolapp/1.1.5-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/charybdis/1.003-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/thepeg/1.9.2p1-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/sherpa/2.2.0-ikhhed5/include/SHERPA-MC:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/QtOpenGL:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/QtGui:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/pythia8/212-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/herwig/6.521-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/QtScript:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/Qt3Support:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/classlib/3.1.3-ikhhed/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/cgal/4.2-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/tkonlinesw/4.0.0-1-ikhhed3/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/Qt:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/QtCore:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/qt/4.8.1/include/QtXml:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/mcdb/1.0.2/interface:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libungif/4.1.4/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libtiff/4.0.3/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libpng/1.6.16/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/geant4/10.00.p03-giojec/include/Geant4:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/frontier_client/2.8.19-ikhhed/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/pcre/8.37/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/boost/1.57.0-ikhhed/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/xz/5.2.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/xrootd/4.0.4-ikhhed2/include/xrootd/private:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/vdt/v0.3.2-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/valgrind/3.11.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/utm/r46131-xsd310-patch/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/toprex/4.23/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/tbb/44_20151115oss/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/tauola/27.121.5/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/sigcpp/2.6.2/include/sigc++-2.0:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/rivet/2.4.0-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/sqlite/3.8.11.1-ikhhed/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/protobuf/2.4.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/pacparser/1.3.5/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/oracle/11.2.0.3.0__10.2.0.4.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/meschach/1.2.pCMS1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libhepml/0.2.1/interface:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/ktjet/1.06-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/jimmy/4.2-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/jemalloc/4.2.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/herwigpp/2.7.1-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/heppdt/3.03.00/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/hector/1.3.4_patch1-ikhhed4/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gsl/1.16/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libjpeg-turbo/1.3.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/giflib/4.2.3/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/xerces-c/2.8.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gdbm/1.10/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/freetype/2.5.3/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/fftw3/3.3.2/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/fftjet/1.5.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/fastjet/3.1.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/expat/2.1.0/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/hepmc/2.06.07/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/dpm/1.8.0.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/dcap/2.47.8/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libxml2/2.9.1/include/libxml2:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/curl/7.47.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/cppunit/1.12.1/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/clhep/2.2.0.4-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/openssl/1.0.2d/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/pythia6/426/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/photos/215.5/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/zlib/1.2.8/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/libuuid/2.22.2/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/castor/2.1.13.9/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/castor/2.1.13.9/include/shift:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/cascade/2.2.04-giojec/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/bz2lib/1.0.6/include:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/python/2.7.11-ikhhed/include/python2.7:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/include/c++/5.3.0:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/include/c++/5.3.0/x86_64-pc-linux-gnu:/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/gcc/5.3.0/include/c++/5.3.0/backward:/usr/local/include:/usr/include"
        try:
            os.execv(sys.argv[0], sys.argv)
        except Exception, exc:
            print 'Failed re-exec:', exc
            sys.exit(1)

    sys.path.append("/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed4/lib")
    import ROOT as r
    r.gROOT.SetBatch(True)

r.gROOT.ProcessLine(".L {0}/plotmaker.cc".format(os.path.realpath(__file__).rsplit("/",1)[0]))

########################################################################
# New TColors
########################################################################
mycolors = []
mycolors.append(r.TColor(11005 , 103 / 255. , 0   / 255. , 31  / 255.))
mycolors.append(r.TColor(11004 , 178 / 255. , 24  / 255. , 43  / 255.))
mycolors.append(r.TColor(11003 , 214 / 255. , 96  / 255. , 77  / 255.))
mycolors.append(r.TColor(11002 , 244 / 255. , 165 / 255. , 130 / 255.))
mycolors.append(r.TColor(11001 , 253 / 255. , 219 / 255. , 199 / 255.))
mycolors.append(r.TColor(11000 , 247 / 255. , 247 / 255. , 247 / 255.))
mycolors.append(r.TColor(11011 , 209 / 255. , 229 / 255. , 240 / 255.))
mycolors.append(r.TColor(11012 , 146 / 255. , 197 / 255. , 222 / 255.))
mycolors.append(r.TColor(11013 , 67  / 255. , 147 / 255. , 195 / 255.))
mycolors.append(r.TColor(11014 , 33  / 255. , 102 / 255. , 172 / 255.))
mycolors.append(r.TColor(11015 , 5   / 255. , 48  / 255. , 97  / 255.))

mycolors.append(r.TColor(3001 , 239 / 255. , 138 / 255. , 98  / 255.))
mycolors.append(r.TColor(3000 , 247 / 255. , 247 / 255. , 247 / 255.))
mycolors.append(r.TColor(3011 , 103 / 255. , 169 / 255. , 207 / 255.))

mycolors.append(r.TColor(5001 , 251 / 255. , 180 / 255. , 174 / 255.))
mycolors.append(r.TColor(5002 , 179 / 255. , 205 / 255. , 227 / 255.))
mycolors.append(r.TColor(5003 , 204 / 255. , 235 / 255. , 197 / 255.))
mycolors.append(r.TColor(5004 , 222 / 255. , 203 / 255. , 228 / 255.))
mycolors.append(r.TColor(5005 , 254 / 255. , 217 / 255. , 166 / 255.))

mycolors.append(r.TColor(7000 ,   0/255. ,   0/255. ,   0/255.))
mycolors.append(r.TColor(7001 , 213/255. ,  94/255. ,   0/255.)) #r
mycolors.append(r.TColor(7002 , 230/255. , 159/255. ,   0/255.)) #o
mycolors.append(r.TColor(7003 , 240/255. , 228/255. ,  66/255.)) #y
mycolors.append(r.TColor(7004 ,   0/255. , 158/255. , 115/255.)) #g
mycolors.append(r.TColor(7005 ,   0/255. , 114/255. , 178/255.)) #b
mycolors.append(r.TColor(7006 ,  86/255. , 180/255. , 233/255.)) #k
mycolors.append(r.TColor(7007 , 204/255. , 121/255. , 167/255.)) #p
mycolors.append(r.TColor(7011 , 110/255. ,  54/255. ,   0/255.)) #alt r
mycolors.append(r.TColor(7012 , 161/255. , 117/255. ,   0/255.)) #alt o
mycolors.append(r.TColor(7013 , 163/255. , 155/255. ,  47/255.)) #alt y
mycolors.append(r.TColor(7014 ,   0/255. , 102/255. ,  79/255.)) #alt g
mycolors.append(r.TColor(7015 ,   0/255. ,  93/255. , 135/255.)) #alt b
mycolors.append(r.TColor(7016 , 153/255. , 153/255. , 153/255.)) #alt k
mycolors.append(r.TColor(7017 , 140/255. ,  93/255. , 119/255.)) #alt p

mycolors.append(r.TColor(9001 ,  60/255. , 186/255. ,  84/255.))
mycolors.append(r.TColor(9002 , 244/255. , 194/255. ,  13/255.))
mycolors.append(r.TColor(9003 , 219/255. ,  50/255. ,  54/255.))
mycolors.append(r.TColor(9004 ,  72/255. , 133/255. , 237/255.))

# Color schemes from Hannsjoerg for WWW analysis
mycolors.append(r.TColor(2001 , 91  / 255. , 187 / 255. , 241 / 255.)) #light-blue
mycolors.append(r.TColor(2002 , 60  / 255. , 144 / 255. , 196 / 255.)) #blue
mycolors.append(r.TColor(2003 , 230 / 255. , 159 / 255. , 0   / 255.)) #orange
mycolors.append(r.TColor(2004 , 180 / 255. , 117 / 255. , 0   / 255.)) #brown
mycolors.append(r.TColor(2005 , 245 / 255. , 236 / 255. , 69  / 255.)) #yellow
mycolors.append(r.TColor(2006 , 215 / 255. , 200 / 255. , 0   / 255.)) #dark yellow
mycolors.append(r.TColor(2007 , 70  / 255. , 109 / 255. , 171 / 255.)) #blue-violet
mycolors.append(r.TColor(2008 , 70  / 255. , 90  / 255. , 134 / 255.)) #violet
mycolors.append(r.TColor(2009 , 55  / 255. , 65  / 255. , 100 / 255.)) #dark violet
mycolors.append(r.TColor(2010 , 120 / 255. , 160 / 255. , 0   / 255.)) #light green
mycolors.append(r.TColor(2011 , 0   / 255. , 158 / 255. , 115 / 255.)) #green
mycolors.append(r.TColor(2012 , 204 / 255. , 121 / 255. , 167 / 255.)) #pink?
