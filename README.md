# Cinder-SdfText
Based on [msdfgen](https://github.com/Chlumsky/msdfgen) by Viktor Chlumský.

## Building for Windows
Clone `cinder` in an adjacent folder.  E.g.:

```
myProject/
    dependencies/
        cinder/
        cinder-sdftext/
    code/
        myProjectSolution/
```

1. Add `cinder.projx` and `cinder-sdftext.projx` to your Visual Studio Solution.
2. Add `config/cinder-sdftext-production.props` to your Visual Studio Solution.
3. Build.
4. Enjoy!

See how the examples are built.  They use the property sheet `cinder-sdftext-example` to get the paths right, but otherwise are the same.

As of 2021 March 31, only the Debug x64 builds of all samples are correctly set up; the Release builds have not been updated.

## Building for OSX and iOS
1. Build lib first in ```xcode```
1. Build samples in ```samples```

## Windows, OSX, and iOS for now! Linux coming soon!

![Basic](https://cdn-standard.discourse.org/uploads/libcinder/optimized/1X/6550b3422474c85a7c46b4bc83c02c1a06bcf7e8_1_626x500.png)

![FontSizes](https://cdn-standard.discourse.org/uploads/libcinder/original/1X/a12d8f8322b86763859022e6f25bfd4b54815828.png)

![StarWars](https://cdn-standard.discourse.org/uploads/libcinder/optimized/1X/4743384cafd790e1bbcbf102288a2623b78c376c_1_690x404.png)

![MeshPages](http://discourse.libcinder.org/uploads/libcinder/original/1X/e411bb26f85e2f82d361c187d183cc11c97b04b3.png)

![StarWars-iOS](https://cdn-standard.discourse.org/uploads/libcinder/original/1X/fe3847ea996e890334f109de30780a8fcd30ff51.png)
