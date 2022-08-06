import os
import sys
import filecmp

def compile_tests():
    tests = [ f.path for f in os.scandir(os.path.dirname(__file__) + "/files/") if f.is_dir() ]
    
    compiled = 1
    for test in tests:
        os.system(os.path.dirname(__file__) + "../bamasm " + test + "/test.baf " + test + "/expected")

        dirs = test.split('/')
        print("✅ Compiled test " + str(compiled) + ": " + dirs[len(dirs) - 1])

        compiled += 1

def run_tests():
    # Get all test subdirs in "files" directory
    tests = [ f.path for f in os.scandir(os.path.dirname(__file__) + "/files/") if f.is_dir() ]
    
    num_done = 1
    for test in tests:
        # Assemble the actual result
        os.system(os.path.dirname(__file__) + "/../bamasm " + test + "/test.baf " + test + "/actual")

        # Compare the expected result with the actual result
        are_equal = filecmp.cmp(test + "/actual", test + "/expected")

        dirs = test.split('/')
        if are_equal:
            print("✅ Test " + str(num_done) + " passed: " + dirs[len(dirs) - 1])
        else:
            print("❌ Test " + str(num_done) + " failed: " + dirs[len(dirs) - 1])
            sys.exit(1)
        
        os.remove(test + "/actual")
        
        num_done += 1

if __name__ == "__main__":
    args = sys.argv
    if len(args) > 1:
        if args[1] == "--compile":
            compile_tests()
        elif args[1] == "--run":
            run_tests()
        else:
            print("Usage: python3 run-tests.py <optional: --compile or --run>")
    else:
        run_tests()