#! /usr/bin/python3
import tempfile
import shutil
import os

def clone_files(files):
    """
    Clone files in the list `files` into a named temporary directory.
    An entry ending with '/' in the list `files` means a directory,
    otherwise it will be treated as a normal file.
    The resulting object is created using `TemporaryDirectory()`.
    """

    tempdir = tempfile.TemporaryDirectory()

    for filename in files:

        newfilename = filename
        if isinstance(filename, tuple):
            newfilename = filename[1]
            filename = filename[0]

        """
        Generate the parent directories for the file to be cloned
        """
        parentdir = os.path.dirname(filename)
        if filename.endswith('/'):
            parentdir = os.path.dirname(parentdir)
        try:
            os.makedirs(os.path.join(tempdir.name, parentdir))
        except FileExistsError as e:
            assert os.path.isdir(e.filename)

        """
        Clone file
        """
        if filename.endswith('/'): # Clone a directory
            shutil.copytree(filename, os.path.join(tempdir.name, filename))
        else:                      # Clone a normal file
            shutil.copy2(filename, os.path.join(tempdir.name, filename))

        """
        Rename file when necessary
        """
        if filename != newfilename:
            filepath = os.path.join(tempdir.name, filename)
            newfilepath = os.path.join(tempdir.name, newfilename)
            os.replace(filepath, newfilepath)

        """
        Clone the metadata of the parent directories
        """
        while parentdir != '':
            parentdirpath = os.path.join(tempdir.name, parentdir)
            os.makedirs(parentdirpath, exist_ok = True)
            shutil.copystat(parentdir, parentdirpath)
            parentdir = os.path.dirname(parentdir)

    return tempdir

def convert_to_asp(filename, commentstr='// '):
    """
    Convert the file `filename` from HRP2 version into ASP version.
    Conversion rules:
    1) Comment out the content in the following case
       '// DEF_HRP2_ONLY'
       'some content' => '// some content'
       '// END_HRP2_ONLY'
    2) Uncomment the content in the following case
       '// DEF_ASP_ONLY'
       '// some content' => 'some content'
       '// END_ASP_ONLY'
    """

    f = open(filename, 'r')
    lines = f.readlines()
    f.close()
    newlines = []

    should_add_comment = False
    should_del_comment = False

    for idx in range(len(lines)):
        line = lines[idx]

        if line.startswith(commentstr + 'DEF_HRP2_ONLY'):
            assert should_add_comment == False
            assert should_del_comment == False
            should_add_comment = True
        elif line.startswith(commentstr + 'END_HRP2_ONLY'):
            assert should_add_comment == True
            should_add_comment = False
        elif line.startswith(commentstr + 'DEF_ASP_ONLY'):
            assert should_add_comment == False
            assert should_del_comment == False
            should_del_comment = True
        elif line.startswith(commentstr + 'END_ASP_ONLY'):
            assert should_del_comment == True
            should_del_comment = False
        else: # Normal line
            if should_add_comment:
                line = commentstr + line
            elif should_del_comment:
                assert line.startswith(commentstr)
                line = line[len(commentstr):]
                newlines.append(line)
            else:
                newlines.append(line)

    f = open(filename, 'w')
    for line in newlines:
        f.write(line)
    f.close()

class Package:
    def __init__(self, name, is_asp, desc):
        self.name = name
        self.is_asp = is_asp
        self.desc = desc

    def make(self):
        tempdir = clone_files(self.files)

        if self.is_asp:
            for root, dirs, files in os.walk(tempdir.name):
                for filename in files:
                    filepath = os.path.join(root, filename)
                    print(filepath)
                    if filename.endswith('.cfg'):
                        convert_to_asp(filepath)
                    elif filename.startswith('Makefile'):
                        convert_to_asp(filepath, '# ')
                    elif filename.endswith('.sh'):
                        convert_to_asp(filepath, '# ')
                    elif filename.endswith('.tf'):
                        convert_to_asp(filepath, '$ ')


        shutil.make_archive(self.name, 'gztar', tempdir.name)

"""
Definitions of packages
"""

hrp2_tsp = Package('ev3_hrp2_tsp', False, 'Target Support Package for HRP2')
hrp2_tsp.files = [
    'mksample.sh',
    'arch/arm_gcc/am1808/',
    'arch/arm_gcc/common/',
    'target/ev3_gcc/ev3.h',
    'target/ev3_gcc/Makefile.target',
    'target/ev3_gcc/target_asm.inc',
    'target/ev3_gcc/target_cfg1_out.h',
    'target/ev3_gcc/target_check.tf',
    'target/ev3_gcc/target_config.c',
    'target/ev3_gcc/target_config.h',
    'target/ev3_gcc/target_kernel.h',
    'target/ev3_gcc/target_mem.cfg',
    'target/ev3_gcc/target_mem.tf',
    'target/ev3_gcc/target_opt.tf',
    'target/ev3_gcc/target_rename.h',
    'target/ev3_gcc/target_serial.c',
    'target/ev3_gcc/target_serial.cfg',
    'target/ev3_gcc/target_serial.h',
    'target/ev3_gcc/target_sil.h',
    'target/ev3_gcc/target_stddef.h',
    'target/ev3_gcc/target_svc.h',
    'target/ev3_gcc/target_syssvc.h',
    'target/ev3_gcc/target_test.h',
    'target/ev3_gcc/target.tf',
    'target/ev3_gcc/target_timer.cfg',
    'target/ev3_gcc/target_timer.h',
    'target/ev3_gcc/target_unrename.h',
#    'target/ev3_gcc/',
]


asp_tsp = Package('ev3_asp_tsp', True, 'Target Support Package for ASP')
asp_tsp.files = [
    'mksample.sh',
    'arch/arm_gcc/am1808/',
    ('arch/arm_gcc/common-asp/', 'arch/arm_gcc/common/'),
    'target/ev3_gcc/ev3.h',
    'target/ev3_gcc/Makefile.target',
    'target/ev3_gcc/target_asm.inc',
    'target/ev3_gcc/target_cfg1_out.h',
    'target/ev3_gcc/target_check.tf',
    'target/ev3_gcc/target_config.c',
    'target/ev3_gcc/target_config.h',
    'target/ev3_gcc/target_kernel.h',
    'target/ev3_gcc/target_mem.cfg',
    'target/ev3_gcc/target_mem.tf',
    'target/ev3_gcc/target_opt.tf',
    'target/ev3_gcc/target_rename.h',
    'target/ev3_gcc/target_serial.c',
    'target/ev3_gcc/target_serial.cfg',
    'target/ev3_gcc/target_serial.h',
    'target/ev3_gcc/target_sil.h',
    'target/ev3_gcc/target_stddef.h',
    'target/ev3_gcc/target_svc.h',
    'target/ev3_gcc/target_syssvc.h',
    'target/ev3_gcc/target_test.h',
    'target/ev3_gcc/target.tf',
    'target/ev3_gcc/target_timer.cfg',
    'target/ev3_gcc/target_timer.h',
    'target/ev3_gcc/target_unrename.h',
#    'target/ev3_gcc/',
]

hrp2_pcp = Package('ev3_hrp2_pcp', False, 'Platform Core Package for HRP2')
hrp2_pcp.files = [
    'mkgyroboy.sh',
    'gyroboy/app.c',
    'gyroboy/app.cfg',
    'gyroboy/app.h',
    'gyroboy/Makefile',
    'include/extsvc_fncode.h',
    'target/ev3_gcc/api/',
    'target/ev3_gcc/drivers/',
    'target/ev3_gcc/ev3main.c',
    'target/ev3_gcc/ev3-pcp.cfg',
    'target/ev3_gcc/Makefile.pcp',
    'target/ev3_gcc/TLSF-2.4.6/',
#    'target/ev3_gcc/',
]

asp_pcp = Package('ev3_asp_pcp', True, 'Platform Core Package for ASP')
asp_pcp.files = [
    'mkgyroboy.sh',
    'gyroboy/app.c',
    'gyroboy/app.cfg',
    'gyroboy/app.h',
    ('gyroboy/Makefile-asp', 'gyroboy/Makefile'),
    'target/ev3_gcc/api/',
    'target/ev3_gcc/drivers/',
    'target/ev3_gcc/ev3main.c',
    'target/ev3_gcc/ev3-pcp.cfg',
    'target/ev3_gcc/Makefile.pcp',
    'target/ev3_gcc/TLSF-2.4.6/',
#    'target/ev3_gcc/',
]

hrp2_plt = Package('hrp2_ev3_platform', False, 'TOPPERS/HRP2 EV3 Platform')
hrp2_plt.files = hrp2_tsp.files + hrp2_pcp.files
hrp2_plt.files.append('cfg/cfg/cfg.exe')

#hrp2_tsp.make()
#asp_tsp.make()
#hrp2_pcp.make()
#asp_pcp.make()

hrp2_plt.make()

#arc = clone_files(file_list)
#shutil.make_archive('ev3-hrp2-tsp', 'gztar', arc.name)

#convert_to_asp('target/ev3_gcc/target_serial.cfg')
