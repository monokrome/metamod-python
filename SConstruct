#####################################################################
# I know that you think you do - but you really don't need to touch
# anything around here. So, don't. --monokrome
###
import os

OUTPUT_FILE_DIR = os.path.abspath('./build/')

SOURCE_FILES = [
    './src/CMetaModPy.cpp',
    './src/CMetaModPy_Hooks.cpp',
    './src/CPythonManager.cpp',
    './src/CPythonPlugin.cpp',
]

INCLUDE_DIRS = [
    # This project's code
    './src',
    './public',

    # Include directories for the Source SDK
    '${HL2SDK}/public',
    '${HL2SDK}/public/engine',
    '${HL2SDK}/public/tier0',
    '${HL2SDK}/public/tier1',
    '${HL2SDK}/public/vstdlib',

    # Include directories for metamod
    '${SMM_ROOT}/${SMM_BASE_DIR}',
    '${SMM_ROOT}/${SMM_BASE_DIR}/sourcehook',

    # Python
    '${PYTHON}',
]

LIBRARIES = [
    'm',
    'dl',
    'python2.6',
]

SOURCE_ENGINES = {
    'unknown': 0,
    'original': 1,
    'episodeone': 2,
    'orangebox': 3,
    'left4dead': 4,
    'darkmessiah': 5,
    'orangeboxvalve': 6,
    'left4dead2': 7,
}

SOURCE_ENGINE_DIRECTORIES = {
    'original': '../hl2sdk/',
    'episodeone': '../hl2sdk-ob-valve/',
    'orangebox': '../hl2sdk-ob/',
    'left4dead': '../hl2sdk-l4d/',
    'darkmessiah': '../hl2sdk-darkmessiah/',
    'orangeboxvalve': '../hl2sdk-ob-valve/',
    'left4dead2': '../hl2sdk-l4d2/',
}

VARIABLES = [
    BoolVariable('RELEASE', 'Disable to compile in debug mode.', 1),
    EnumVariable(
        'ENGINE',
        'Choose an engine to compile for',
        'orangeboxvalve',
        allowed_values=SOURCE_ENGINES
    ),
    PathVariable(
        'HL2SDK',
        'The path to where your HL2SDK files are located',
        '../hl2sdk/'
    ),
    PathVariable(
        'SMM_ROOT',
        'Path to your MetaMod:Source code',
        '../mmsource/'
    ),
    PathVariable(
        'SRCDS',
        'Path to srcds on the system',
        '~/srcds/'
    ),
    PathVariable(
        'PYTHON',
        'Path to the Python sourcecode',
        '../python/'
    ),
]

DEFINITIONS = {
    '_LINUX': None,
    'HAVE_STDINT_H': None,

    "stricmp": "strcasecmp",
    "_stricmp": "strcasecmp",
    "_strnicmp": "strncasecmp",
    "strnicmp": "strncasecmp",
    "_snprintf": "snprintf",
    "_vsnprintf": "vsnprintf",
    "_alloca": "alloca",
    "strcmpi": "strcasecmp",
}

COMPILER_OPTIONS = [
#    '-Wall',
#    '-Werror',
#    '-m32',
#    '-Wno-non-virtual-dtor',
#    '-fno-exceptions -fno-rtti',
#    '-fno-strict-aliasing',
#    '-Wno-switch',
#    '-Wno-unused',
#    '-Wno-uninitialized ',
#    '-Wno-non-virtual-dtor',
    '-mfpmath=sse',
    '-msse2',
    '-fno-exceptions',
    '-fno-rtti',
    '-fvisibility=hidden',
    '-fvisibility-inlines-hidden',
    '-O0',
    '-g',
    '-ggdb3',
]

LINKER_OPTIONS = [
    '-m32',
    '-static-libgcc',
    '-debug',
]


# Manage our build options
vars = Variables('build.py')

# Add our environment variables
for variable in VARIABLES:
    DEFINITIONS[variable[0]] = '"${%s}"' % variable[0]
    vars.Add(variable)

# Set up our environment
env = Environment(variables=vars)

# Use our source engine constants
for engine in SOURCE_ENGINES:
    DEFINITIONS['SOURCE_ENGINE_%s' % engine.upper()] = SOURCE_ENGINES[engine]    

DEFINITIONS['SOURCE_ENGINE'] = 'SOURCE_ENGINE_%s' % env.get('ENGINE').upper()

# Decide whether or not we need to use the legacy API
env['SMM_BASE_DIR'] = 'core'

if SOURCE_ENGINES[env.get('ENGINE')] < SOURCE_ENGINES['orangebox']:
    env['SMM_BASE_DIR'] = 'core-legacy'

env['NAME'] = 'MetaModPy'
env['DESCRIPTION'] = 'A Python extension for MetaMod'

env['CXX'] = 'gcc-4.1'
env['CPPPATH'] = INCLUDE_DIRS
env['CPPDEFINES'] = DEFINITIONS
env['CPPFLAGS'] = ' '.join(COMPILER_OPTIONS)
env['LINKFLAGS'] = ' '.join(LINKER_OPTIONS)
env['LIBPATH'] = env.get('PYTHON')
env['LIBS'] = LIBRARIES
env['HL2SDK'] = SOURCE_ENGINE_DIRECTORIES[env['ENGINE']]
env['SHLIBPREFIX'] = ''

# Reset some default stuff
env['SHCCFLAGS'] = ''

env.Prepend(
    SHLIBSUFFIC='.ext',
    _LIBFLAGS='${SRCDS}/bin/vstdlib_i486.so ${SRCDS}/bin/tier0_i486.so '
)

#########################
# Finally, we can compile our library
OUTPUT_FILE = os.path.join(OUTPUT_FILE_DIR, 'metamodpy')
env.SharedLibrary(OUTPUT_FILE, SOURCE_FILES)

print OUTPUT_FILE
