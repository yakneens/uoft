import org.xml.sax.HandlerBase;
import org.xml.sax.helpers.ParserFactory;
import org.xml.sax.Parser;
import org.xml.sax.XMLReader;
import org.xml.sax.SAXParseException;

/** Validates an XML document */
/*
* @author Matt Medland
* @date 09/09/2001
*/
public class Validator extends HandlerBase {
    private static final int RC_VALID = 0;
    private static final int RC_INVALID = 1;
    private static final int RC_FATAL = 2;

    private int rc = RC_VALID; // return code
    private boolean verbose = false;

    // the XML document we're validating
    private String docName = null;

    // methods overriden from HandlerBase
    public void warning(SAXParseException ex) { handleParseException(ex); }
    public void error(SAXParseException ex) { handleParseException(ex); }
    public void fatalError(SAXParseException ex) { handleParseException(ex); }

    private void handleParseException(Exception e) {
        setReturnCode(RC_INVALID);

        if (isVerbose()) {
            if (e instanceof SAXParseException) {
                System.out.print(
                 getLocationString((SAXParseException) e) + ": ");
            }

            System.out.println(e);
        }
    }

    /** Returns a string of the location. */
    private String getLocationString(SAXParseException ex) {
        StringBuffer str = new StringBuffer();

        String systemId = ex.getSystemId();
        if (systemId != null) {
            int index = systemId.lastIndexOf('/');
            if (index != -1)
                systemId = systemId.substring(index + 1);
            str.append(systemId);
        }
        str.append(':');
        str.append(ex.getLineNumber());
//        str.append(':');
//        str.append(ex.getColumnNumber());

        return str.toString();

    } // getLocationString(SAXParseException):String

    public int getReturnCode() { return rc; }
    public void setReturnCode(int rc) { this.rc = rc; }
    public boolean isVerbose() { return verbose; }
    public void setVerbose(boolean verbose) { this.verbose = verbose; }
    public String getXMLDocumentName() { return docName; }
    public void setXMLDocumentName(String docName) { this.docName = docName; }

    private void parseArgs(String[] args) {
        if (args.length < 1 || args.length > 2) {
            System.err.println("incorrect number of arguments");
            printUsage();
            System.exit(RC_FATAL);
        }

        String xmlDoc = null;

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-v")) {
                setVerbose(true);
            }
            else if (xmlDoc != null) {
                System.err.println("can only validate one file at a time");
                printUsage();
                System.exit(RC_FATAL);
            }
            else {
                xmlDoc = args[i];
            }
        }

        if (xmlDoc == null) {
            System.err.println("you must specify an XML document");
            printUsage();
            System.exit(RC_FATAL);
        }
        if (!xmlDoc.endsWith(".xml")) {
            System.err.println("XML file must end in '.xml'");
            printUsage();
            System.exit(RC_FATAL);
        }

        setXMLDocumentName(xmlDoc);
    }

    private static void printUsage() {
        System.out.println(
         "Usage: java -cp <classpath> Validator [-v] <XML document>");
    }

    public static void main(String[] args) {
        Validator validator = new Validator();
        Parser parser = null;

        validator.parseArgs(args);

        if (validator.isVerbose()) {
            System.out.println("Validating " +
             validator.getXMLDocumentName() + "...");
        }

        // create the SAX parser
        try {
            parser = ParserFactory.makeParser(
             "org.apache.xerces.parsers.SAXParser");
        } catch (Exception e) {
            System.err.println("Caught exception creating parser: " + e);
            System.exit(RC_FATAL);
        }

        parser.setDocumentHandler(validator);
        parser.setErrorHandler(validator);

        try { // want this parser to load the DTD and do error checking
            ((XMLReader) parser).setFeature(
             "http://xml.org/sax/features/validation",
             true);
            ((XMLReader) parser).setFeature(
             "http://apache.org/xml/features/nonvalidating/load-external-dtd",
             true);
        } catch (Exception e) {
            System.err.println(
             "Caught exception setting parser features: " + e);
            System.exit(RC_FATAL);
        }

        try {
            parser.parse(validator.getXMLDocumentName());
        } catch (Exception e) {
            validator.handleParseException(e);
        }

        if (validator.getReturnCode() == 0 && validator.isVerbose()) {
            System.out.println(validator.getXMLDocumentName() +
            " passed validation test.");
        }

        System.exit(validator.getReturnCode());
    }
}

// Because the "getLocationString" method was taken from an Xerces example I
// have to include the Liscense agreement below.

/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999,2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.apache.org.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */