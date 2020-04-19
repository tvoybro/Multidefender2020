<?
print_r($argv);

$filename = isset($argv[1]) ? $argv[1] : '';
if (!$filename || !file_exists(__DIR__ . '/' . $filename)) {
	echo 'file not found';
} else {
	$img = imagecreatefrompng(__DIR__ . '/' . $filename);
	imagesavealpha($img, true);
	ob_start();
	imagepng($img);
	$imgBin = ob_get_clean();
	$imgMime = 'data:image/png;base64,' . base64_encode($imgBin);
	file_put_contents(__DIR__ . '/' . $filename . '.mime', $imgMime);
}