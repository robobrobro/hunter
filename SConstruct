env = Environment(
    BUILD_ROOT = '#build',
    BUILD_DIR = '$BUILD_ROOT/$MODE',

    CCFLAGS = ['-Wall', '-Werror', '-Wextra', '-std=c11'],
    CPPPATH = ['src', 'src/$MODE'],
)

dbg_env = env.Clone(MODE='debug')
dbg_env.Append(CCFLAGS=['-g'])

rel_env = env.Clone(MODE='release')
rel_env.Append(CCFLAGS=['-O3'])

for env in (dbg_env, rel_env):
    env.SConscript(
        dirs = '.',
        variant_dir = env.subst('$BUILD_DIR'),
        duplicate = False,
        exports = {'env': env.Clone()},
    )
