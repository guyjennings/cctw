function aggregatedMask(fileName, threshold) {
    processor.loadData(fileName);
    processor.subtractDark();

    processor.newMaskStack();
    processor.hideMaskRange(threshold, 1e10);
    processor.andMask();
}

function aggregateMask(dir, pat, threshold) {
    processor.newMaskStack();

    var fileList = matchFiles(dir, pat);

    for (i in fileList) {
        var fileName = dir+fileList[i]
        print("Loading file ", fileName);
        processor.loadData(dir+fileList[i]);
        processor.subtractDark();
        processor.hideMaskRange(threshold, 1e10);
    }
}
