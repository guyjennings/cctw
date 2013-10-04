cctw.calculateDependencies();

arr = [100, 50, 30, 20, 25, 15, 10];

for (key in arr) {
  var sz = arr[key];

  transformer.blocksLimit = sz;

  transformer.transform();

  print("Size", sz, "Wall Time", transformer.wallTime, "Max", transformer.blocksMax);
}
