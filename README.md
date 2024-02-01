# DPF Test Plugins
Playground to experiment with DPF plugin framework.


### Cmake commands
To include TorchScript:

```shell
$ cd build

# if installed PyTorch with pip:
$ cmake -DCMAKE_PREFIX_PATH=`python3 -c 'import torch;print(torch.utils.cmake_prefix_path)'` ..

# otherwise:
$ cmake -DCMAKE_PREFIX_PATH=/absolute/path/to/libtorch ..

$ cmake --build . --config Release
```