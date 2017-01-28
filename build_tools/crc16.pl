# calculate a CRC16-CCITT value for a binary file
# and output as hex text string

use Digest::CRC;

open(FILE, $ARGV[0]) or die;
binmode(FILE);
  
$ctx = Digest::CRC->new(type=>"crcccitt");
$ctx->addfile(*FILE);
  
close(FILE);

print "0x" . $ctx->hexdigest