subdir('mids')

test('pop', pop_exe)

foreach test_mid : test_mids
    test('list: @0@'.format(test_mid), 
         list_exe,
         args: test_mid)
endforeach

foreach test_mid : test_mids
    test('info: @0@'.format(test_mid), 
         info_exe,
         args: test_mid)
endforeach

foreach test_mid : test_mids
    test('ext: @0@'.format(test_mid), 
         ext_exe,
         args: test_mid)
endforeach

foreach test1_mid : test_mids
    foreach test2_mid : test_mids
        test('analyze: @0@ @1@'.format(test1_mid, test2_mid), 
             analyze_exe,
             args: [test1_mid, test2_mid])
    endforeach
endforeach

