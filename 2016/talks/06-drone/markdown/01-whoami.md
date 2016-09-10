**$ whoami**

* @braincode on twitter, @brainstorm on github.
* An ~~outsider~~ newbie in the HW/RE community.
* Bioinformatician, so python is awesome (hugs @trufae).
* Recent hobbies: playing with MCUs such as Atmega8 and the mighty ESP8266/ESP32.
* Swedish, catalan, (heading towards Australia?).


I deal with this...
-------------------

<pre><code class="hljs" data-trim contenteditable>
@SEQ_ID
GATTTGGGGTTCAAAGCAGTATCGATCAAATAGTAAATCCATTTGTTCAACTCACAGTTT
+
!''*((((***+))%%%++)(%%%%).1***-+*''))**55CCF>>>>>>CCCCCCC65
@SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72
GGGTGATGGCCGCTGCCGATGGCGTCAAATCCCACCAAGTTACCCTTAACAACTTAAGGGTTTTCAAATAGA
+SRR001666.1 071112_SLXA-EAS1_s_7:5:1:817:345 length=72
IIIIIIIIIIIIIIIIIIIIIIIIIIIIII9IG9ICIIIIIIIIIIIIIIIIIIIIDIIIIIII>IIIIII/
@SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72
GTTCAGGGATACGACGTTTGTATTTTAAGAATCTGAAGCAGAAGTCGATGATAATACGCGTCGTTTTATCAT
+SRR001666.2 071112_SLXA-EAS1_s_7:5:1:801:338 length=72
IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII6IBIIIIIIIIIIIIIIIIIIIIIIIGII>IIIII-I)8I
</code></pre>

([FASTQ format](https://en.wikipedia.org/wiki/FASTQ_format))


...and this
-----------

<pre><code class="hljs" data-trim contenteditable>
#CHROM	POS	ID	REF	ALT	QUAL	FILTER	INFO	FORMAT	NA00001	NA00002	NA00003
20	14370	rs6054257	G	A	29	PASS	NS=3;DP=14;AF=0.5;DB;H2	GT:GQ:DP:HQ	0|0:48:1:51,51	1|0:48:8:51,51	1/1:43:5:.,.
20	17330	.	T	A	3	q10	NS=3;DP=11;AF=0.017	GT:GQ:DP:HQ	0|0:49:3:58,50	0|1:3:5:65,3	0/0:41:3
20	1110696	rs6040355	A	G,T	67	PASS	NS=2;DP=10;AF=0.333,0.667;AA=T;DB	GT:GQ:DP:HQ	1|2:21:6:23,27	2|1:2:0:18,2	2/2:35:4
</code></pre>

(genomic) [Variant Call Format](https://samtools.github.io/hts-specs/VCFv4.3.pdf)


... and this too
-----------------

![structural_variations](res/sv.png)

[Structural Variations](https://en.wikipedia.org/wiki/Structural_variation)


Sometimes this [as seen in radare-extras BCL support](https://github.com/radare/radare2-extras/tree/master/bcl)
--------------------------------------------------------------------------------------------

![illumina_bcl_radare](res/illumina_bcl_radare.png)

[Illumina(tm) BCL format](http://support.illumina.com/content/dam/illumina-support/documents/documentation/software_documentation/bcl2fastq/bcl2fastq_letterbooklet_15038058brpmi.pdf)

Note:

Kudos to @trufae for implementing most of the spec for shits and giggles in radare-extras.
