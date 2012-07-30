<?php
$files = scandir('.');
foreach ($files as $file)
{
	$ext = strstr($file, '.');
	if ($ext == '.h' || $ext == '.cpp')
	{
		$class = strstr($file, '.', true);
		$cont = file_get_contents($file);
		$cont = str_replace('membrane', $class, $cont);
		$cont = str_replace('Membrane', ucfirst($class), $cont);
		$cont = str_replace('MEMBRANE', strtoupper($class), $cont);
		file_put_contents($file, $cont);
	}
}
?>