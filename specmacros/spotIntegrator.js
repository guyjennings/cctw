function analyze(fileName) {
  processor.loadData(fileName);
  processor.subtractDark();

  var i1 = processor.integrateRectangle( 423,1167, 560,1268)[0];
  var i2 = processor.integrateRectangle(1480, 464,1580, 608)[0];
  var i3 = processor.integrateRectangle(1800,1290,1950,1470)[0];

  return [i1, i2, i3]
}

function analyzeSequence(dir, pat) {
  var fileList = matchFiles(dir,pat);
  var d=[];

  for (i in fileList) {
    var res = analyze(dir+fileList[i]);
    d.push([i, res[0], res[1], res[2]]);
  }

  for (i in d) {
    print(d[i][0], "\t", d[i][1], "\t", d[i][2], "\t", d[i][3]);
  }

  return d;
}
