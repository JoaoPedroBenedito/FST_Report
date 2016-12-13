	.text	
		
	.align	2
	.globl	read_image
	.ent	read_image
read_image:
	addik	r1,r1,-4	
	sw	r15,r0,r1	
	addk	r3,r0,r0
$L2:
	addik	r18,r0,8191
	cmp	r18,r3,r18
	blti	r18,$L1
	get r5, rfsl0
	addik	r4,r3, image
	sb	r5, r0, r4				
	addik	r3,r3,1	
	bri	$L2
$L1:
	lw	r15,r0,r1
	addik	r1,r1,4
	rtsd	r15,8 	 
	nop
	.end	read_image

	.align	2
	.globl	write_image
	.ent	write_image
write_image:
	addik	r1,r1,-4	
	sw	r15,r0,r1	
	addk	r3,r0,r0
$L3:
	addik	r18,r0,8191
	cmp	r18,r3,r18
	blti	r18,$L4
	lbui r5,r3, image 
	put r5, rfsl0
	addik	r3,r3,1	
	bri	$L3
$L4:
	lw	r15,r0,r1
	addik	r1,r1,4
	rtsd	r15,8 	 
	nop
	.end	write_image
	
	.align	2
	.globl	main
	.ent	main
main:
$L23:
  #brlid	r15,_initialize_timer	
	#nop

	brlid	r15,read_image
	nop
	
	##obtencao do histograma

	##limpar histograma
	addik	 	r21,r0,hist	##r21 com endereço histograma
	addk 		r24,r0,r0	##numero cores em r24
$L12:	
	sw		r0,r21,r0	##hist[i]=0
	addik 	r21,r21,4	##i++
	addik		r24,r24,1	##cores++
	rsubik	r28,r24,256
	bnei		r28,$L12	##ainda nao chegou ao fim do histograma
	##-----------
	
	addik 	r21,r0,image	
	addk 		r23,r0,r0	##para numero de posicoes na imagem

$L13:
	lbu		r24,r21,r0	##r24 e um pixel
	andi		r24,r24,0x00ff	##mascara 1 byte
	bslli 	r24,r24,2	##x4 pois o histograma e 4 bytes
	addik		r25,r24,hist	##posicao no histograma a incrementar
	lw		r26,r0,r25	##obtem o valor do histograma para o pixel apurado
	addik		r26,r26,1	##incrementa o valor do histograma
	sw		r26,r0,r25	##guarda novo valor
	addik 	r21,r21,1	##proximo pixel
	addik		r23,r23,1	##menos um pixel para ler
	rsubik	r28,r23,8192	##chegou ao fim da imagem salta
	bnei		r28,$L13

	##acomulacao do histograma
	addik		r24,r0,hist	
	addik		r25,r0,1	##para percorrer o numero de posicoes do histograma
	lw		r26,r0,r24	##r26 e o hist anterior
	addik		r24,r24,4	##proxima posicao
$L14:	
	lw		r27,r0,r24	##r27	e o hist actual
	addk		r26,r26,r27	##acomula
	sw		r26,r0,r24	##guarda acomulacao
	addik		r24,r24,4	##proxima posicao
	addik		r25,r25,1	##menos uma posicao para acomular
	rsubik	r28,r25,256	##se chegou ao fim do histograma passa a frente
	bnei		r28,$L14

	##equalizacao
	addik		r21,r0,image
	addk		r23,r0,r0
$L15:
	lbu		r24,r0,r21	##pixel a equalizar
	andi		r24,r24,0x00ff	##mascara 1 byte
	bslli 	r24,r24,2	##x4 pois o hit tem 4 bytes
	addik		r25,r24,hist	##encontra a posicao de acomulacao do histograma necessaria
	lw		r26,r0,r25	##obtem o valor da acomulacao	
	bsrli 	r26,r26,5	##executa cores/N
	rsubik	r28,r26,255	##teste de saturacao
	bgei		r28,$L16
	addik		r26,r0,255	##se passou a representacao
$L16:
	sb		r26,r0,r21	##guarda pixel equalizado
	addik		r21,r21,1	##proximo pixel a tratar
	addik		r23,r23,1	##menos um pixel para tratar
	rsubik	r28,r23,8192	##teste de fim de imagem
	bnei		r28,$L15
	
	##escreve imagem
$L22:

	brlid	r15,write_image	
	nop

  #brlid	r15,_print_cycles
	#nop

	bri	$L23
	.end	main

	.bss
	.comm	image,8192	
	.comm	hist,1024
	