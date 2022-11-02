# Comando para gerar vídeo

ffmpeg -framerate 4 -i gen_%04d.png -c:v libx264 -r 30 -pix_fmt yuv420p  -vf "pad=ceil(iw/2)*2:ceil(ih/2)*2" gen.mp4

