# Cinder-SdfText

Based on [msdfgen](https://github.com/Chlumsky/msdfgen) by Viktor Chlumský.

## Building for Windows

Clone `cinder` in an adjacent folder. E.g.:

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

See how the examples are built. They use the property sheet `cinder-sdftext-example` to get the paths right, but otherwise are the same.

As of 2021 March 31, only the Debug x64 builds of all samples are correctly set up; the Release builds have not been updated.

## Usage

To get started quickly, start with the `Basic` example to get some text on the screen.

To better understand exactly how various options affect type, it's highly recommended to check out the `TypographyAnatomy` sample. This will show you how various options affect the look and feel of the type.

Some quirks that are useful to call out for anyone not familiar with the block:

### Leading

`leading` (line spacing) is measured in percentage. A value of `1.2` means 120% of your font size (20% extra space between lines).

This block supports two styles of leading: full leading and half-leading. Half-leading only has an effect when you use a `fitRect` (see below) and will create a space equal to 50% of the leading before the first line of text.

### Tracking

`tracking` is measured in 1/1000 of an `em`. A tracking value of `50` would equal 50/1000 = 0.05, or 5%.

### Fit Rect vs Bounding Box

A `fitRect` is a rectangle that you would like the text to fit into. A `fitRect` is used with `SdfText::drawStringWrapped` and optionally with `SdfText::drawString`. When using a `fitRect`, the text will be placed _inside_ of the rectangle; when using a `baseline` the text will sit on _top_ of the baseline. These will (and should!) give you different placements of the text.

A bounding box (given by `measureStringBoundsWrapped` or `measureStringBounds`) will give you the actual bounding box for the text. This will include any ascenders, descenders, ligatures, and adjustments to tracking and/or leading. This is the real, honest-to-god, bounding box of the text as rendered and has should _not_ be used as a measure of typographic parameters. This box will not give you accurate information about line height, leading, etc.!

The `TypographyAnatomy` example provides a clear example of `fitRect` vs bounding box in the display options.

### Anchoring

Using `drawString` with and without a `fitRect` creates substantially different effects.

With a `fitRect`, anchoring (the position you place the text at) gives the top-left corner of the text box _containing_ the text. Without a `fitRect`, your position will be the baseline (bottom of the text). In practical terms, with a baseline the text appears _above_ the position, with a `fitRect` it will show up `below`. In `TypographyAnatomy` you can see how as you increase the leading, the last two text samples (drawn with `fitRect`s) will stay anchored to the baseline above them.

Alignment will also work differently. With a `fitRect`, the `SdfText::Alignment` options will align the text _inside_ of the box to be flush left, centered, or flush right. When using a baseline, the alignment setting will align the text so the the anchor point is either the bottom-left, bottom-center, or bottom-right of the text.

## Examples

![Basic](https://cdn-standard.discourse.org/uploads/libcinder/optimized/1X/6550b3422474c85a7c46b4bc83c02c1a06bcf7e8_1_626x500.png)

![FontSizes](https://cdn-standard.discourse.org/uploads/libcinder/original/1X/a12d8f8322b86763859022e6f25bfd4b54815828.png)

![StarWars](https://cdn-standard.discourse.org/uploads/libcinder/optimized/1X/4743384cafd790e1bbcbf102288a2623b78c376c_1_690x404.png)

![MeshPages](http://discourse.libcinder.org/uploads/libcinder/original/1X/e411bb26f85e2f82d361c187d183cc11c97b04b3.png)

![StarWars-iOS](https://cdn-standard.discourse.org/uploads/libcinder/original/1X/fe3847ea996e890334f109de30780a8fcd30ff51.png)
