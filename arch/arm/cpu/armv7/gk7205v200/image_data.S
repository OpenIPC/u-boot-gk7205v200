.section .image,#alloc
.globl	input_data
/*gzip source addr must be 16 bytes aligned*/
.balign 16
input_data:
.incbin   "image_data.gzip"

.globl    input_data_end
input_data_end:
