Because I always forget...

## Nosetests

```
> cd tests/nose
> nosetests -s
```

## Rostests

Refer to http://wiki.ros.org/rostest/Commandline

```
> rostest --text dslam_ecto_utils load_yaml_from_param.test
```

Or directly:

```
> rostest --text load_yaml_from_param.test
```

## Gtests

From the build dir

```
# Build them
> make tests
# Run them
> make test
```

From yujin_make:

```
# Build them
> yujin_make -t
# Run them
> yujin_make -r
```

Singly, with detailed output, switch to the binary directory and run ctest, e.g.

```
> cd /opt/dslam/dslam_desktop/build/dslam/dslam_3rdparty/param_loader/tests/gtests
> ctest -VV
```

Or you can even run them directly from the `devel/lib/_package_name` folder so long as they don't rely
on resources (e.g. yamls) off in the build directories somewhere.

