.region DATA {
	.word	x			0x01F7;
	.array	list[6]		0, 1, 2, 3, 4, 5;
	.string	str			"Hello World";
}

.region PROGRAM {
	.entry {
		LTA		10;
		LTR		rA, $0x2000;
		ADD		rA;
		STM		$0x2001;
	}
}