.data {
	.word	x			0x01F7;
	.array	list[6]		0, 1, 2, 3, 4, 5;
	// string literals
	.string	str			"Hello World";
	.string str2		"What's good?";
}

.program {
	.entry { //entry point for program
		LTA		10;
		LTR		rA  $0x2000;	"test"
		/* this
section is
		commented
					out */
		ADD		rA;
		STM		$0x2001;		"test"
	}
}